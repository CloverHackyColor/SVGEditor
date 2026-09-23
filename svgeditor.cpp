#include "svgeditor.h"

#include <QDir>
#include <QFile>
#include <QTextStream>
#include <QFileInfoList>
#include <QFileDialog>
#include <QListWidgetItem>
#include <QImage>
#include <QGraphicsScene>
#include <QPixmap>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QTimer>
#include <QResizeEvent>
#include <algorithm>

#include "nanosvg.h"
#include "SvgDebugLog.h"

SVGEditor::SVGEditor(QWidget* parent)
  : QMainWindow(parent), fileList(nullptr)
{
  ui.setupUi(this);
  SvgDebugSetOutput(ui.textEditLog);

  // Создаём список файлов
  fileList = new QListWidget(this);
  fileList->setObjectName(QString::fromUtf8("fileList"));
  fileList->setFocusPolicy(Qt::StrongFocus);

  // Первоначально откроем текущую папку приложения
  currentFolder = QDir::currentPath();
  populateFileList(currentFolder);

  if (fileList->count() > 0) {
    fileList->setCurrentRow(0);
    fileList->setFocus();
  }

  createRelations();
}

SVGEditor::~SVGEditor()
{
  SvgDebugSetOutput(nullptr);
}

void SVGEditor::resizeEvent(QResizeEvent* event)
{
  QMainWindow::resizeEvent(event);
  emit windowResized();
}

bool SVGEditor::eventFilter(QObject* obj, QEvent* event)
{
  // Если graphicsView изменил размер (например, при ресайзе окна), перерисовываем SVG
  if (obj == ui.graphicsView && event->type() == QEvent::Resize) {
    QTimer::singleShot(0, this, &SVGEditor::onDraw);
  }
  return QMainWindow::eventFilter(obj, event);
}


void SVGEditor::populateFileList(const QString& path)
{
  fileList->clear();
  QDir dir(path);
  QStringList filters;
  filters << "*.svg" << "*.xml"; // << "*.txt"; // по вкусу — добавьте нужные расширения
  QFileInfoList list = dir.entryInfoList(filters, QDir::Files, QDir::Name);
  for (const QFileInfo& fi : list) {
    QListWidgetItem* it = new QListWidgetItem(fi.fileName(), fileList);
    it->setData(Qt::UserRole, fi.filePath()); // хранить полный путь
    fileList->addItem(it);
  }
}

void SVGEditor::onGenerate()
{
  ui.textEdit->setText("<?xml version=\"1.0\" encoding=\"UTF - 8\"?>\n"
                       "<svg width = \"1000\" height = \"800\" viewBox = \"0 0 1000 800\" xmlns = \"http://www.w3.org/2000/svg\" "
                       "xmlns:clover = \"https://github.com/CloverHackyColor/CloverBootloader\" >\n</svg>");
}

void SVGEditor::onDraw()
{
  QByteArray svgData = ui.textEdit->toPlainText().toUtf8();
  if (svgData.isEmpty()) return;

  NSVGparser* parser = nsvg__parse(svgData.data(), 72, 1.f);
  if (!parser || !parser->image) {
    if (parser) nsvg__deleteParser(parser);
    return;
  }

  NSVGimage* SVGimage = parser->image;
  //DBG("SVG from Edit: width=%d height=%d\n", (int)(SVGimage->width), (int)(SVGimage->height));

  {
    QSize viewSize = ui.graphicsView->viewport()->size();
    INTN Width = viewSize.width() - 5;
    INTN Height = viewSize.height() - 5;
    if (Width <= 0 || Height <= 0) {
      nsvg__deleteParser(parser);
      return;
    }

    NSVGrasterizer* rast = nsvg__createRasterizer();
    float Scale, ScaleX, ScaleY;

    // Rasterize
    QImage image(Width, Height, QImage::Format_RGBA8888);
    image.fill(Qt::transparent);
    if (SVGimage->width <= 0) SVGimage->width = (float)Width;
    if (SVGimage->height <= 0) SVGimage->height = (float)Height;

    ScaleX = (Width - 10) / SVGimage->width;
    ScaleY = (Height - 10) / SVGimage->height;
    Scale = (ScaleX > ScaleY) ? ScaleY : ScaleX;
    float tx = 2; 
    float ty = 2; 
    //DBG("timing rasterize start tx=%f ty=%f\n", tx, ty); //the aim is measure duration
    nsvgRasterize(rast, SVGimage, tx, ty, Scale, Scale, image.bits(), (int)Width, (int)Height, (int)Width * 4);

    //DBG("timing rasterize end\n");
    nsvg__deleteRasterizer(rast);

    if (!ui.graphicsView->scene()) {
      ui.graphicsView->setScene(new QGraphicsScene(ui.graphicsView));
    }
    ui.graphicsView->scene()->clear();
    // Явно задаем размер сцены, чтобы исключить авто-центрирование и смещения
    ui.graphicsView->scene()->setSceneRect(0, 0, Width, Height);
    ui.graphicsView->scene()->addPixmap(QPixmap::fromImage(image));
    // Сбрасываем любую трансформацию, чтобы изображение не масштабировалось
    ui.graphicsView->resetTransform();
  }

  nsvg__deleteParser(parser);
}

void SVGEditor::onFileSelected(int row)
{
  if (row < 0 || !fileList) return;

  QString fullPath = fileList->item(row)->data(Qt::UserRole).toString();
  if (fullPath.isEmpty()) return;

  QFile file(fullPath);
  if (!file.open(QIODevice::ReadOnly)) return;

  // Читаем все байты и явно конвертируем из UTF-8
  QByteArray bytes = file.readAll();
  QString text = QString::fromUtf8(bytes);
  ui.textEdit->setPlainText(text);

  ui.textEditLog->clear();
  onDraw();
}

void SVGEditor::onChooseFolder()
{
  QString dir = QFileDialog::getExistingDirectory(this, tr("Select Folder"), currentFolder,
    QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
  if (dir.isEmpty()) return;

  currentFolder = dir;
  populateFileList(currentFolder);

  if (fileList->count() > 0) {
    fileList->setCurrentRow(0);
    fileList->setFocus();
  }
}

void SVGEditor::onSave()
{
  // Пример сохранения в UTF-8 (можно заменить на сохранение в исходный путь)
  QString filePath = QFileDialog::getSaveFileName(this, tr("Save File"), currentFolder,
    tr("SVG Files (*.svg);;All Files (*)"));
  if (filePath.isEmpty()) return;

  QFile file(filePath);
  if (!file.open(QIODevice::WriteOnly)) return;

  QByteArray out = ui.textEdit->toPlainText().toUtf8(); // явная конвертация в UTF-8
  file.write(out);
}

void SVGEditor::createRelations()
{
  // при смене выбранной строки — загружаем файл
  connect(fileList, &QListWidget::currentRowChanged, this, &SVGEditor::onFileSelected);

  // кнопка "Folder" открывает диалог выбора папки
  connect(ui.pushButtonLoad, &QPushButton::clicked, this, &SVGEditor::onChooseFolder);

  connect(ui.pushButtonSave, &QPushButton::clicked, this, &SVGEditor::onSave);

  // Create new SVG — тоже в нижнем ряду
  connect(ui.pushButtonNew, &QPushButton::clicked, this, &SVGEditor::onGenerate);

  connect(ui.tabWidget, &QTabWidget::currentChanged, this, [this](int index) {
    if (index == ui.tabWidget->indexOf(ui.ViewTab)) {
      QTimer::singleShot(0, this, &SVGEditor::onDraw);
    }
  });


  // КРИТИЧНО: Удаляем внутренний layout tabWidget, чтобы он не мешал setGeometry
  if (ui.tabWidget->layout()) {
    QLayoutItem* item;
    while ((item = ui.tabWidget->layout()->takeAt(0)) != nullptr) {}
    delete ui.tabWidget->layout();
  }

  // 2. Логика ручного пересчета геометрии при изменении размера окна
  connect(this, &SVGEditor::windowResized, this, [this]() {
    int newWidth = width();
    int newHeight = height();

    const int margin = 10;
    const int logH = 70;

    // --- Кнопка "Folder" (pushButtonLoad) ---
    // Оставляем в левом верхнем углу, размер НЕ меняем (сохраняем "логотип")
    int btnX = margin;
    int btnY = ui.pushButtonLoad->y();
    //int btnW = ui.pushButtonLoad->width();
    int btnH = ui.pushButtonLoad->height();
    
    // --- TabWidget ---
    // Располагаем СТРОГО справа от кнопки, чтобы ничего не перекрывалось
    // 1. TabWidget
    ui.tabWidget->setGeometry(210, 10, newWidth - 220, newHeight - logH - 30);
    QRect tabGeomNew = ui.tabWidget->geometry();

    // --- graphicsView внутри tabWidget ---
    // Учитываем высоту заголовка вкладок (примерно 25px) и небольшие отступы
    // 5. GraphicsView внутри tabWidget
    // ВАЖНО: Координаты задаются ОТНОСИТЕЛЬНО tabWidget, а не главного окна!
    int tabBarHeight = ui.tabWidget->tabBar()->height();
    if (tabBarHeight <= 0) tabBarHeight = 25; // Фоллбэк, если tabBar еще не инициализирован
    int gvMargin = 5;

    ui.graphicsView->setGeometry(gvMargin, gvMargin,
      tabGeomNew.width() - 4 * gvMargin,
      tabGeomNew.height() - tabBarHeight - 4 * gvMargin);

    // --- Список файлов ---
    // Под кнопкой "Folder", той же ширины, растягивается по высоте
    int listX = btnX;
    int listY = btnY + btnH + margin;
    int listW = tabGeomNew.x() - margin * 2;
    if (listW < 120) listW = 120; // Минимальная ширина

    // Высота списка: всё свободное место до кнопок Save/New
    //int btnAreaH = ui.pushButtonSave->height() + margin;
    int listH = tabGeomNew.height() + margin - listY;
    if (listH < 50) listH = 50;

    fileList->setGeometry(listX, listY, listW, listH);

    // --- Кнопки New и Save ---
    // Прижимаются к низу (под списком файлов), сдвигаются вместе с ним
    int btnNewY = listY + listH + margin;
    int btnSaveY = btnNewY;

    ui.pushButtonNew->setGeometry(listX, btnNewY, 75, ui.pushButtonNew->height());
    ui.pushButtonSave->setGeometry(listX + 100, btnSaveY, 75, ui.pushButtonSave->height());

    // --- Лог-панель ---
    // Внизу, под tabWidget
    int logX = tabGeomNew.x();
    int logY = tabGeomNew.y() + tabGeomNew.height() + margin;
    int logW = tabGeomNew.width();
    ui.textEditLog->setGeometry(logX, logY, logW, logH);

    // --- Перерисовка SVG ---
    // Используем таймер, чтобы дать время на применение новой геометрии viewport
    QTimer::singleShot(0, this, &SVGEditor::onDraw);
    });

  ui.graphicsView->setAlignment(Qt::AlignLeft | Qt::AlignTop);

  // Первичный вызов ресайза для установки начальной геометрии при запуске
  emit windowResized();

}
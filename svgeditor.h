#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_svgeditor.h"
#include <QListWidget>
#include <QString>
#include "EfiCommon.h"

class SVGEditor : public QMainWindow
{
  Q_OBJECT

public:
  SVGEditor(QWidget* parent = nullptr);
  ~SVGEditor();

  char *FileData = NULL;
  int listX = 0;
  QRect bgeom;
  QRect tabGeom;

private slots:
  void onGenerate();
  void onFileSelected(int row);
  void onChooseFolder();
  void onSave();
  void onDraw();

signals:
  void windowResized();

protected:
  void resizeEvent(QResizeEvent* event) override;
  bool eventFilter(QObject* obj, QEvent* event) override;

private:
  Ui::SVGEditorClass ui;
  QListWidget* fileList;
  QString currentFolder;
  QString currentFilePath;

  void createRelations();
  void populateFileList(const QString& path);
};

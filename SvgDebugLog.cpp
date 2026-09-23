#include "SvgDebugLog.h"

#include <QMetaObject>
#include <QPointer>
#include <QString>

#include <cstdarg>
#include <cstdio>

namespace {
  QPointer<QTextEdit> g_output;
}

void SvgDebugSetOutput(QTextEdit* output)
{
  g_output = output;
}

void SvgDebugLog(const char* format, ...)
{
  char buffer[4096];

  va_list args;
  va_start(args, format);
  std::vsnprintf(buffer, sizeof(buffer), format, args);
  va_end(args);

  const QString message =
    QString::fromLocal8Bit(buffer).trimmed();

  QPointer<QTextEdit> output = g_output;
  if (!output)
    return;

  QMetaObject::invokeMethod(
    output,
    [output, message]() {
      if (output)
        output->append(message);
    },
    Qt::QueuedConnection);
}
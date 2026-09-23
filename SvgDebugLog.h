#pragma once

#include <QTextEdit>

void SvgDebugSetOutput(QTextEdit* output);
void SvgDebugLog(const char* format, ...);
#ifndef MVCGLOBAL_H
#define MVCGLOBAL_H

#include <QtCore/qglobal.h>

QT_BEGIN_NAMESPACE

#ifndef QT_STATIC
#  if defined(QT_BUILD_MVC_LIB)
#    define Q_MVC_EXPORT Q_DECL_EXPORT
#  else
#    define Q_MVC_EXPORT Q_DECL_IMPORT
#  endif
#else
#  define Q_MVC_EXPORT
#endif

QT_END_NAMESPACE

#endif

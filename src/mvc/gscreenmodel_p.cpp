#include "gscreenmodel_p.h"
#include "gscreenmodel.h"

ScreenModelPrivate::ScreenModelPrivate(ScreenModel *q)
    : QObject(0)
    , q_ptr(q)
    , m_dpi(0)
    , m_textScaleFactor(1.)
    , m_contentScaleFactor(1.)
    , m_devicePixelRatio(1)
{
}

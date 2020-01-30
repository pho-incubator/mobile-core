#include "gscreenmodel_default_p.h"
#include "gscreenmodel.h"

ScreenModelPrivate *ScreenModelPrivate::create(ScreenModel *q)
{
    return new ScreenModelDefaultPrivate(q);
}

ScreenModelDefaultPrivate::ScreenModelDefaultPrivate(ScreenModel *q)
    : ScreenModelPrivate(q)
{
    m_canvasSize.setWidth(320);
    m_canvasSize.setHeight(480);
}

void ScreenModelDefaultPrivate::init()
{
    calculateDPI();
    QScreen *screen = qApp->primaryScreen();

    // may be it is redundant :)
#if defined(Q_OS_OSX) || defined(Q_OS_WIN)
    connect(screen, SIGNAL(logicalDotsPerInchChanged(qreal)), this, SLOT(calculateDPI()));
#else
    connect(screen, SIGNAL(physicalDotsPerInchChanged(qreal)), this, SLOT(calculateDPI()));
#endif
}

void ScreenModelDefaultPrivate::calculateDPI()
{
    Q_Q(ScreenModel);

    QScreen *screen = qApp->primaryScreen();

    // this is the case that we are not using ApplicationWindow qml
#if defined(Q_OS_OSX) || defined(Q_OS_WIN)
    int dpi = screen->logicalDotsPerInch() * screen->devicePixelRatio();
#else
    int dpi = screen->physicalDotsPerInch() * screen->devicePixelRatio();
#endif

    q->setDpi(dpi);

    qCDebug(LScreenModel) << "DPI" << dpi
                          << "PhyscialDotsPerInch" << screen->physicalDotsPerInch()
                          << "LogicalDotsPerInch" << screen->logicalDotsPerInch()
                          << "Device Pixel Ratio" << screen->devicePixelRatio();
}

void ScreenModelDefaultPrivate::showStatusBar(bool animated)
{
    Q_UNUSED(animated);
}

void ScreenModelDefaultPrivate::hideStatusBar(bool animated)
{
    Q_UNUSED(animated);
}



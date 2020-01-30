#include "gscreenmodel_ios_p.h"
#include "gscreenmodel.h"
#include "gmvcfacade.h"
#include <QQuickWindow>
#import <UIKit/UIKit.h>

#define IS_IPHONE_4 (fabs((double)[[UIScreen mainScreen]bounds].size.height - (double)480) < DBL_EPSILON)
#define IS_IPHONE_5 (fabs((double)[[UIScreen mainScreen]bounds].size.height - (double)568) < DBL_EPSILON)
#define IS_IPHONE_6 (fabs((double)[[UIScreen mainScreen]bounds].size.height - (double)667) < DBL_EPSILON)
#define IS_IPHONE_6_PLUS (fabs((double)[[UIScreen mainScreen]bounds].size.height - (double)736) < DBL_EPSILON)
#define IS_IPAD (UI_USER_INTERFACE_IDIOM() == UIUserInterfaceIdiomPad)

ScreenModelPrivate *ScreenModelPrivate::create(ScreenModel *q)
{
    return new ScreenModelIosPrivate(q);
}

ScreenModelIosPrivate::ScreenModelIosPrivate(ScreenModel *q)
    : ScreenModelPrivate(q)
{
    if (IS_IPHONE_4) {
        m_canvasSize.setWidth(320);
        m_canvasSize.setHeight(480);
    } else if (IS_IPHONE_5) {
        m_canvasSize.setWidth(320);
        m_canvasSize.setHeight(568);
    } else if (IS_IPHONE_6) {
        m_canvasSize.setWidth(375);
        m_canvasSize.setHeight(667);
    } else if (IS_IPHONE_6_PLUS) {
        m_canvasSize.setWidth(360);
        m_canvasSize.setHeight(640);
    } else if (IS_IPAD) {
        m_canvasSize.setWidth(768);
        m_canvasSize.setHeight(1024);
    }
}

void ScreenModelIosPrivate::init()
{
    calculateDPI();

    QScreen *screen = qApp->primaryScreen();
    connect(screen, &QScreen::geometryChanged, this, &ScreenModelIosPrivate::calculateDPI);
    // may be it is redundant :)
    connect(screen, &QScreen::physicalDotsPerInchChanged, this, &ScreenModelIosPrivate::calculateDPI);
}

void ScreenModelIosPrivate::calculateDPI()
{
    Q_Q(ScreenModel);

    QScreen *screen = qApp->primaryScreen();
    // we are using ApplicationWindow for mobile so we don't multiply with devicePixelRatio
    int dpi = screen->physicalDotsPerInch();
    q->setDpi(dpi);

    m_devicePixelRatio = qApp->devicePixelRatio();
    emit q->devicePixelRatioChanged();

    qCDebug(LScreenModel) << "DPI" << dpi
                          << "PhyscialDotsPerInch" << screen->physicalDotsPerInch()
                          << "LogicalDotsPerInch" << screen->logicalDotsPerInch()
                          << "Device Pixel Ratio" << screen->devicePixelRatio();
}

void ScreenModelIosPrivate::showStatusBar(bool animated)
{
    /*if (animated)
        [[UIApplication sharedApplication] setStatusBarHidden:YES withAnimation:UIStatusBarAnimationSlide];
    else
        [[UIApplication sharedApplication] setStatusBarHidden:YES];*/

    Q_UNUSED(animated);

    Q_Q(ScreenModel);
    MvcFacade *facade = qobject_cast<MvcFacade *>(q->parent());
    QObject *top = qobject_cast<QQmlApplicationEngine *>(facade->engine())->rootObjects().at(0);
    QQuickWindow *window = qobject_cast<QQuickWindow *>(top);
    window->setWindowState(Qt::WindowMaximized);
}

void ScreenModelIosPrivate::hideStatusBar(bool animated)
{
    /*if (animated)
        [[UIApplication sharedApplication] setStatusBarHidden:NO withAnimation:UIStatusBarAnimationSlide];
    else
        [[UIApplication sharedApplication] setStatusBarHidden:NO];*/

    Q_UNUSED(animated);

    Q_Q(ScreenModel);
    MvcFacade *facade = qobject_cast<MvcFacade *>(q->parent());
    QObject *top = qobject_cast<QQmlApplicationEngine *>(facade->engine())->rootObjects().at(0);
    QQuickWindow *window = qobject_cast<QQuickWindow *>(top);
    window->setWindowState(Qt::WindowFullScreen);
}

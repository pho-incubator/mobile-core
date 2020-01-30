#include "gscreenmodel_android_p.h"
#include "gscreenmodel.h"
#include <QtAndroidExtras>

ScreenModelPrivate *ScreenModelPrivate::create(ScreenModel *q)
{
    return new ScreenModelAndroidPrivate(q);
}

ScreenModelAndroidPrivate::ScreenModelAndroidPrivate(ScreenModel *q)
    : ScreenModelPrivate(q)
{
    m_canvasSize.setWidth(320);
    m_canvasSize.setHeight(480);
}

void ScreenModelAndroidPrivate::init()
{
    calculateDPI();

    QScreen *screen = qApp->primaryScreen();
    connect(screen, &QScreen::geometryChanged, this, &ScreenModelAndroidPrivate::calculateDPI);
}

void ScreenModelAndroidPrivate::calculateDPI()
{
    Q_Q(ScreenModel);

    QScreen *screen = qApp->primaryScreen();

    // https://bugreports.qt-project.org/browse/QTBUG-35701
    // recalculate dpi for Android
    QAndroidJniEnvironment env;
    QAndroidJniObject activity = QtAndroid::androidActivity();
    QAndroidJniObject resources = activity.callObjectMethod("getResources", "()Landroid/content/res/Resources;");
    if (env->ExceptionCheck()) {
        env->ExceptionDescribe();
        env->ExceptionClear();
        return;
    }

    QAndroidJniObject displayMetrics = resources.callObjectMethod("getDisplayMetrics", "()Landroid/util/DisplayMetrics;");
    if (env->ExceptionCheck()) {
        env->ExceptionDescribe();
        env->ExceptionClear();
        return;
    }
    int dpi = displayMetrics.getField<int>("densityDpi");

    q->setDpi(dpi);

    m_devicePixelRatio = screen->size().width() / 320; // we 'll use for assets
    emit q->devicePixelRatioChanged();

    qCDebug(LScreenModel) << "DPI" << dpi
                          << "PhyscialDotsPerInch" << screen->physicalDotsPerInch()
                          << "LogicalDotsPerInch" << screen->logicalDotsPerInch()
                          << "Device Pixel Ratio" << screen->devicePixelRatio();
}


void ScreenModelAndroidPrivate::showStatusBar(bool animated)
{
    Q_UNUSED(animated);
}

void ScreenModelAndroidPrivate::hideStatusBar(bool animated)
{
    Q_UNUSED(animated);
}

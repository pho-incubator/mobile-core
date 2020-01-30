#include "screen_model.h"
#include "screen_model_p.h"
#ifdef Q_OS_ANDROID
#include <QtAndroidExtras>
#endif
#include <QGuiApplication>
#include <QScreen>

Q_LOGGING_CATEGORY(LScreenModel, "Mvc.ScreenModel")

ScreenModelPrivate::ScreenModelPrivate(ScreenModel *q)
    : QObject(0)
    , q_ptr(q)
    , m_dpi(0)
    , m_textScaleFactor(1.)
    , m_contentScaleFactor(1.)
{
}

void ScreenModelPrivate::init()
{
    calculateDPI();
    QScreen *screen = qApp->primaryScreen();
    connect(screen, SIGNAL(logicalDotsPerInchChanged(qreal)), this, SLOT(calculateDPI()));
}

void ScreenModelPrivate::calculateDPI()
{
    Q_Q(ScreenModel);

    QScreen *screen = qApp->primaryScreen();
    int dpi = screen->physicalDotsPerInch() * screen->devicePixelRatio();

    qWarning() << "DPI" << dpi << "PhyscialDotsPerInch" << screen->physicalDotsPerInch() << "Device Pixel Ratio" << screen->devicePixelRatio();

// https://bugreports.qt-project.org/browse/QTBUG-35701
// recalculate dpi for Android
#if defined(Q_OS_IOS)
    dpi = screen->physicalDotsPerInch();
#elif defined(Q_OS_ANDROID)
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
    dpi = displayMetrics.getField<int>("densityDpi");
#elif defined(Q_OS_WIN) || defined(Q_OS_OSX)
    dpi = screen->logicalDotsPerInch() * screen->devicePixelRatio();
#endif

    qWarning() << "Last DPI" << dpi;
    q->setDpi(dpi);
}

const char *ScreenModel::NAME = "ScreenModel";

ScreenModel::ScreenModel(QObject *parent)
    : MvcModel(parent)
    , d_ptr(new ScreenModelPrivate(this))
{
}

ScreenModel::~ScreenModel()
{
    delete d_ptr;
}

qreal ScreenModel::dp(qreal value)
{
    Q_D(ScreenModel);
    return value * d->m_contentScaleFactor;
}

qreal ScreenModel::sp(qreal value)
{
    Q_D(ScreenModel);
    return value * d->m_contentScaleFactor * d->m_textScaleFactor;
}

int ScreenModel::dpi() const
{
    Q_D(const ScreenModel);
    return d->m_dpi;
}

void ScreenModel::setDpi(int dpi)
{
    Q_D(ScreenModel);
    if (d->m_dpi != dpi) {
        d->m_dpi = dpi;
        d->m_contentScaleFactor = dpi / 160.;

        emit dpiChanged();
        emit contentScaleFactorChanged();
    }
}

qreal ScreenModel::contentScaleFactor() const
{
    Q_D(const ScreenModel);
    return d->m_contentScaleFactor;
}

qreal ScreenModel::textScaleFactor() const
{
    Q_D(const ScreenModel);
    return d->m_textScaleFactor;
}

void ScreenModel::setTextScaleFactor(qreal value)
{
    Q_D(ScreenModel);
    if (value != d->m_textScaleFactor) {
        if (value <= 0) {
            qCWarning(LScreenModel) << "Tried to set invalid text scale factor:" << value;
            return;
        }

        d->m_textScaleFactor = value;
        emit textScaleFactorChanged();
    }
}

void ScreenModel::init()
{
    Q_D(ScreenModel);
    d->init();
}

void ScreenModel::apply(const QVariantMap &config)
{
    if (config.contains(QStringLiteral("dpi")))
        setDpi(config.value(QStringLiteral("dpi")).toInt());

    if (config.contains(QStringLiteral("textScaleFactor")))
        setTextScaleFactor(config.value(QStringLiteral("textScaleFactor")).toReal());
}

const char *ScreenModel::name()
{
    return NAME;
}


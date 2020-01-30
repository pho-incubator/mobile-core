#include "gscreenmodel.h"
#include "gscreenmodel_p.h"
#include <QGuiApplication>
#include <QScreen>

Q_LOGGING_CATEGORY(LScreenModel, "GroupsInc.Mvc.ScreenModel")

const char *ScreenModel::NAME = "ScreenModel";

ScreenModel::ScreenModel(MvcFacade *parent)
    : MvcModel(parent)
    , d_ptr(ScreenModelPrivate::create(this))
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
        // d->m_contentScaleFactor = dpi / 160.;
        d->m_contentScaleFactor = (int) (dpi / 160);

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

int ScreenModel::devicePixelRatio() const
{
    Q_D(const ScreenModel);
    return d->m_devicePixelRatio;
}

void ScreenModel::showStatusBar(bool animated)
{
    Q_D(ScreenModel);
    d->showStatusBar(animated);
}

void ScreenModel::hideStatusBar(bool animated)
{
    Q_D(ScreenModel);
    d->hideStatusBar(animated);
}

void ScreenModel::setCanvasSize(const QRect &rect)
{
    Q_D(ScreenModel);
    if (d->m_canvasSize != rect) {
        d->m_canvasSize = rect;
        emit canvasSizeChanged();
    }
}

QRect ScreenModel::canvasSize() const
{
    Q_D(const ScreenModel);
    return d->m_canvasSize;
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


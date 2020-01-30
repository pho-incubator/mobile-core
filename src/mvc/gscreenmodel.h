#ifndef SCREENMODEL_H
#define SCREENMODEL_H

#include <QObject>
#include <QLoggingCategory>
#include "gmvcmodel.h"
#include <qqml.h>
#include <QRect>

Q_DECLARE_LOGGING_CATEGORY(LScreenModel)

class ScreenModelPrivate;

class Q_MVC_EXPORT ScreenModel : public MvcModel
{
    Q_OBJECT
    Q_PROPERTY(int dpi READ dpi NOTIFY dpiChanged)
    Q_PROPERTY(qreal contentScaleFactor READ contentScaleFactor NOTIFY contentScaleFactorChanged)
    Q_PROPERTY(qreal textScaleFactor READ textScaleFactor NOTIFY textScaleFactorChanged)
    Q_PROPERTY(qreal devicePixelRatio READ devicePixelRatio NOTIFY devicePixelRatioChanged)
    Q_PROPERTY(QRect canvasSize READ canvasSize WRITE setCanvasSize NOTIFY canvasSizeChanged)

public:
    static const char *NAME;

    explicit ScreenModel(MvcFacade *parent = 0);
    ~ScreenModel();

    Q_INVOKABLE qreal dp(qreal value);
    Q_INVOKABLE qreal sp(qreal value);

    int dpi() const;
    void setDpi(int dpi);

    qreal contentScaleFactor() const;
    qreal textScaleFactor() const;
    void setTextScaleFactor(qreal value);
    int devicePixelRatio() const;

    Q_INVOKABLE void showStatusBar(bool animated = true);
    Q_INVOKABLE void hideStatusBar(bool animated = true);

    void setCanvasSize(const QRect &rect);
    QRect canvasSize() const;

    void init();
    const char *name();
    void apply(const QVariantMap &config);

Q_SIGNALS:
    void dpiChanged();
    void contentScaleFactorChanged();
    void textScaleFactorChanged();
    void devicePixelRatioChanged();
    void canvasSizeChanged();

private:
    Q_DECLARE_PRIVATE(ScreenModel)
    ScreenModelPrivate *d_ptr;

};

QML_DECLARE_TYPE(ScreenModel)

#endif // SCREEN_MODEL_H

#ifndef SCREENMODEL_P_H
#define SCREENMODEL_P_H

#include <QObject>
#include <QGuiApplication>
#include <QScreen>
#include "gmvc_global.h"

class ScreenModel;

class Q_MVC_EXPORT ScreenModelPrivate : public QObject
{
    Q_OBJECT
public:
    ScreenModelPrivate(ScreenModel *q);

    virtual void init() = 0;
    static ScreenModelPrivate *create(ScreenModel *q);

    virtual void showStatusBar(bool animated) = 0;
    virtual void hideStatusBar(bool animated) = 0;

public slots:
    virtual void calculateDPI() = 0;

protected:
    Q_DECLARE_PUBLIC(ScreenModel)
    ScreenModel *q_ptr;

    int m_dpi;
    qreal m_textScaleFactor;
    qreal m_contentScaleFactor;
    int m_devicePixelRatio;
    QRect m_canvasSize;
};

#endif // SCREEN_MODEL_P_H

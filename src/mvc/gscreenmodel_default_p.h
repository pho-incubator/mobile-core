#ifndef SCREENMODEL_DEFAULT_P_H
#define SCREENMODEL_DEFAULT_P_H

#include <QObject>
#include "gmvc_global.h"
#include "gscreenmodel_p.h"

class ScreenModel;

class Q_MVC_EXPORT ScreenModelDefaultPrivate : public ScreenModelPrivate
{
    Q_OBJECT
public:
    ScreenModelDefaultPrivate(ScreenModel *q);
    void init();
    void showStatusBar(bool animated);
    void hideStatusBar(bool animated);

public slots:
    void calculateDPI();
};

#endif // SCREENMODEL_DEFAULT_P_H

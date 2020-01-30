#ifndef MVCMODEL_H
#define MVCMODEL_H

#include <QObject>
#include "gmvc_global.h"

class MvcFacade;

class Q_MVC_EXPORT MvcModel : public QObject
{
    Q_OBJECT
public:
    explicit MvcModel(MvcFacade *parent = 0);

    virtual void init() = 0;
    virtual const char *name() = 0;
    virtual void apply(const QVariantMap &config) = 0;

Q_SIGNALS:
    void registered();

};

#endif // MVCMODEL_H

#include "gmvcfacade.h"
#include "qqml.h"
#include <QQmlContext>
#include "gscreenmodel.h"
#include "gassetmodel.h"
#include "gapimodel.h"
#include <QByteArray>

Q_LOGGING_CATEGORY(LMvc, "GroupsInc.Mvc")

MvcFacadePrivate::MvcFacadePrivate(MvcFacade *q)
    : QObject(0)
    , q_ptr(q)
    , m_package("GroupsIncMvcApp")
    , m_majorVersion(MAJOR_VERSION)
    , m_minorVersion(MINOR_VERSION)
    , m_inited(false)
    , m_systemModelsRegistered(false)
    , m_engine(0)
{
}

MvcFacadePrivate::~MvcFacadePrivate()
{
}

MvcFacade *MvcFacade::m_instance = 0;

MvcFacade::MvcFacade()
    : QObject(0)
    , d_ptr(new MvcFacadePrivate(this))
{
}

MvcFacade *MvcFacade::instance()
{
    if (!m_instance)
        m_instance = new MvcFacade();

    return m_instance;
}

void MvcFacade::init()
{
    Q_D(MvcFacade);
    if (d->m_inited)
        return;

    QMap<uint, MvcModel *>::iterator it = d->m_modelMap.begin();
    const QMap<uint, MvcModel *>::const_iterator end = d->m_modelMap.constEnd();
    for (; it != end; ++it)
        it.value()->init();

    d->m_inited = true;
}

void MvcFacade::bind()
{
    Q_D(MvcFacade);
    if (!d->m_engine) {
        qCCritical(LMvc) << "setEngine must be called first.";
        return;
    }

    d->m_engine->setNetworkAccessManagerFactory(&(this->networkAccessManagerFactory));

    if (!d->m_mainQml.isEmpty())
        connect(d->m_engine, SIGNAL(objectCreated(QObject*,QUrl)), this, SLOT(lateInit(QObject*,QUrl)));
    else
        init();
}

void MvcFacade::lateInit(QObject *, const QUrl &url)
{
    Q_D(MvcFacade);
    if (d->m_mainQml == url)
        init();
}

const char *MvcFacade::package() const
{
    Q_D(const MvcFacade);
    return d->m_package;
}

int MvcFacade::majorVersion() const
{
    Q_D(const MvcFacade);
    return d->m_majorVersion;
}

int MvcFacade::minorVersion() const
{
    Q_D(const MvcFacade);
    return d->m_minorVersion;
}

void MvcFacade::setEngine(QQmlApplicationEngine *engine)
{
    Q_D(MvcFacade);
    d->m_engine = engine;
}

QQmlApplicationEngine *MvcFacade::engine() const
{
    Q_D(const MvcFacade);
    return d->m_engine;
}

void MvcFacade::setMainQml(const QUrl &url)
{
    Q_D(MvcFacade);
    d->m_mainQml = url;
}

QUrl MvcFacade::mainQml() const
{
    Q_D(const MvcFacade);
    return d->m_mainQml;
}

QVariantMap & MvcFacade::config()
{
    Q_D(MvcFacade);
    return d->m_config;
}

void MvcFacade::registerSystemModels()
{
    Q_D(MvcFacade);
    if (d->m_systemModelsRegistered)
        return;

    if (d->m_inited) {
        qCCritical(LMvc) << "System models cannot be register after application loads.";
        return;
    }

    registerModel<ScreenModel>(new ScreenModel(this));
    registerModel<AssetModel>(new AssetModel(this));

    d->m_systemModelsRegistered = true;
}

MvcModel *MvcFacade::model(const char *name) const
{
    Q_D(const MvcFacade);
    uint key = qHash(QLatin1String(name));
    if (d->m_modelMap.contains(key))
        return d->m_modelMap.value(key);

    return 0;
}

bool MvcFacade::hasModel(const char *name) const
{
    Q_D(const MvcFacade);
    uint key = qHash(QLatin1String(name));

    return d->m_modelMap.contains(key);
}

void MvcFacade::addModel(MvcModel *model)
{
    Q_D(MvcFacade);
    uint key = qHash(QLatin1String(model->name()));

    d->m_modelMap.insert(key, model);
}

QString MvcFacade::base64ToString(const QString &base64String)
{
    return QString::fromUtf8(QByteArray::fromBase64(base64String.toUtf8()));
}

QString MvcFacade::stringToBase64(const QString &normalString)
{
    return QString::fromUtf8(normalString.toUtf8().toBase64());
}

MvcFacade::~MvcFacade()
{
    foreach (MvcModel *model, d_ptr->m_modelMap)
        model->deleteLater();

    delete d_ptr;
}

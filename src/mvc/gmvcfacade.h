#ifndef MVCFACADE_H
#define MVCFACADE_H

#include <QObject>
#include <QLoggingCategory>
#include <QQmlApplicationEngine>
#include <QVariantMap>
#include "gmvc_global.h"
#include "gmvcmodel.h"
#include <qqml.h>
#include <QQmlContext>
#include <QQmlEngine>
#include <QJSEngine>
#include <qqmlnetworkaccessmanagerfactory.h>
#include <QNetworkAccessManager>
#include <QNetworkRequest>

Q_DECLARE_LOGGING_CATEGORY(LMvc)

#define MAX_DEPTH_LEVEL 10

#define GMVC_DEFINE_MODEL(T) \
    static inline QObject *g_createMvcModel_##T(QQmlEngine *, QJSEngine *) { \
        MvcFacade *facade = MvcFacade::instance(); \
        if (!facade->hasModel(#T)) { \
            qWarning() << #T << "model is not registered. Trying to register now."; \
            facade->registerModel<T>(new T(facade)); \
        } \
        return facade->model(#T); \
    }

#define GMVC_MODEL(T) g_createMvcModel_##T

static inline QString propertyName(const char *name)
{
    QString qName = QString::fromLatin1(name);
    QString propertyName = qName.at(0).toLower() + qName.mid(1);

    return propertyName;
}

typedef QObject *(*MvcCreateCallback)(QQmlEngine *, QJSEngine *);

class MvcFacade;

class MvcNetworkAccessManager : public QNetworkAccessManager
{
    Q_OBJECT
public:
    explicit MvcNetworkAccessManager(QObject *parent = 0)
        : QNetworkAccessManager(parent) {}

    QNetworkReply *createRequest(Operation op, const QNetworkRequest &request, QIODevice *outgoingData = 0)
    {
        QNetworkRequest req(request);
        req.setRawHeader("User-Agent", "Mozilla/5.0 (Macintosh; Intel Mac OS X 10_10_1) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/39.0.2171.95 Safari/537.36");

        return QNetworkAccessManager::createRequest(op, req, outgoingData);
    }
};

class MvcNetworkAccessManagerFactory : public QQmlNetworkAccessManagerFactory
{
public:
    QNetworkAccessManager *create(QObject *parent)
    {
        return new MvcNetworkAccessManager(parent);
    }
};

class Q_MVC_EXPORT MvcFacadePrivate : public QObject
{
    Q_OBJECT
public:
    MvcFacadePrivate(MvcFacade *q);
    ~MvcFacadePrivate();

private:
    Q_DECLARE_PUBLIC(MvcFacade)
    MvcFacade *q_ptr;

    const char *m_package;

    int m_majorVersion;
    int m_minorVersion;
    bool m_inited;
    bool m_systemModelsRegistered;

    QQmlApplicationEngine *m_engine;

    QMap<uint, MvcModel *> m_modelMap;

    QVariantMap m_config;

    QUrl m_mainQml;
};

class Q_MVC_EXPORT MvcFacade : public QObject
{
    Q_OBJECT
public:
    MvcFacade();
    ~MvcFacade();

    static MvcFacade *instance();

    void bind();

    QVariantMap & config();

    const char *package() const;
    int majorVersion() const;
    int minorVersion() const;

    void setEngine(QQmlApplicationEngine *engine);
    QQmlApplicationEngine *engine() const;

    void setMainQml(const QUrl &url);
    QUrl mainQml() const;

    template <typename T>
    inline void registerModel(MvcModel *model, MvcCreateCallback callback = 0)
    {
        if (hasModel(model->name()))
            return;

        Q_D(MvcFacade);

        Q_ASSERT(d->m_engine != 0);
        Q_ASSERT(d->m_engine->rootObjects().size() == 0); // ensure nothing loaded

        T *originalObj = qobject_cast<T *>(model);
        Q_ASSERT(originalObj != 0);

        QString name = QString::fromLatin1(originalObj->name());
        if (d->m_config.contains(name))
            originalObj->apply(d->m_config.value(name).toMap());

        addModel(model);

        if (callback)
            qmlRegisterSingletonType<T>(d->m_package, d->m_majorVersion, d->m_minorVersion,
                                            originalObj->name(), callback);
        qRegisterMetaType<T *>();
    }

    Q_INVOKABLE MvcModel *model(const char *name) const;
    Q_INVOKABLE bool hasModel(const char *name) const;

    void registerSystemModels();

    Q_INVOKABLE QString base64ToString(const QString &base64String);
    Q_INVOKABLE QString stringToBase64(const QString &normalString);

protected:
    void init();
    void addModel(MvcModel *model);

public Q_SLOTS:
    void lateInit(QObject *, const QUrl &url);

private:
    static MvcFacade *m_instance;

    MvcNetworkAccessManagerFactory networkAccessManagerFactory;

    Q_DECLARE_PRIVATE(MvcFacade)
    MvcFacadePrivate *d_ptr;
};

QML_DECLARE_TYPE(MvcFacade)

#endif // MVCFACADE_H

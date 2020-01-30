#include "gapimodel.h"
#include "gapimodel_p.h"
#include "gapimodel_format_p.h"
#include <QNetworkCookieJar>
#include <QTranslator>

Q_LOGGING_CATEGORY(LApiModel, "GroupsInc.Mvc.ApiModel")

static inline QVariantMap mergeDefaults(const QVariantMap &prime, const QVariantMap &d)
{
    QVariantMap map = static_cast<QVariantMap>(prime);
    QVariantMap data = static_cast<QVariantMap>(d);

    for (QVariantMap::iterator it = data.begin(); it != data.end(); ++it)
        if (!map.contains(it.key()))
            map.insert(it.key(), it.value());

    return map;
}

ApiFormatError::ApiFormatError(FormatError error)
{
    this->error = error;
}

QString ApiFormatError::errorString() const
{
    QString error;
    switch (this->error) {
    case ApiFormatError::NoError:
        error = QObject::tr("No error.");
        break;
    case ApiFormatError::ParseError:
        error = QObject::tr("Error occured while parsing process.");
        break;
    case ApiFormatError::AssambleError:
        error = QObject::tr("Error occured while assambling process.");
        break;
    }

    return error;
}

const char *ApiModel::NAME = "ApiModel";

ApiModel::ApiModel(MvcFacade *parent)
    : MvcModel(parent)
    , d_ptr(new ApiModelPrivate(this))
{
}

ApiModel::~ApiModel()
{
    delete d_ptr;
}

quint32 ApiModel::api(const QString &route, const QVariantMap &data)
{
    Q_D(ApiModel);
    if (!d->m_routes.contains(route)) {
        qCWarning(LApiModel) << route << "could not found for API call.";
        return 0;
    }

    QVariantMap routeInfo = d->m_routes.value(route).toMap();
    QUrl url;

    if (routeInfo.contains(QLatin1String("url")))
        url = QUrl(routeInfo.value(QLatin1String("url")).toString());
    else {
        Q_ASSERT(!d->m_baseUrl.isEmpty());
        url = d->m_baseUrl;
    }

    if (routeInfo.contains(QLatin1String("requires"))) {
        QStringList requires = routeInfo.value(QLatin1String("requires")).toStringList();
        int i = 0;
        int len = requires.size();

        for (; i < len; i++) {
            if (!data.contains(requires.at(i))) {
                qCWarning(LApiModel) << requires.at(i) << "is a required value and it is not defined. Call will not be applied for " << route;
                return 0;
            }
        }
    }

    qCDebug(LApiModel) << "Requesting API Route" << route;

    QVariantMap mData;
    if (routeInfo.contains(QLatin1String("defaults")) && routeInfo.value(QLatin1String("defaults")).type() == QVariant::Map)
         mData = mergeDefaults(data, routeInfo.value(QLatin1String("defaults")).toMap());

    ApiModel::ApiMethod method = static_cast<ApiModel::ApiMethod>(routeInfo.value(QLatin1String("method")).toInt());

    return request(method, url, mData.isEmpty() ? data : mData, route);
}

quint32 ApiModel::request(ApiModel::ApiMethod method, const QUrl &url, const QVariantMap &data, const QString &identifier)
{
    Q_D(ApiModel);
    return d->request(method, url, data, identifier);
}

quint32 ApiModel::post(const QUrl &url, const QVariantMap &post)
{
    Q_D(ApiModel);
    return d->request(ApiModel::METHOD_POST, url, post);
}

quint32 ApiModel::post(const QVariantMap &post)
{
    Q_D(ApiModel);
    Q_ASSERT(!d->m_baseUrl.isEmpty());
    return d->request(ApiModel::METHOD_POST, d->m_baseUrl, post);
}

quint32 ApiModel::get(const QUrl &url, const QVariantMap &params)
{
    Q_D(ApiModel);
    return d->request(ApiModel::METHOD_GET, url, params);
}

quint32 ApiModel::get(const QVariantMap &params)
{
    Q_D(ApiModel);
    Q_ASSERT(!d->m_baseUrl.isEmpty());
    return d->request(ApiModel::METHOD_GET, d->m_baseUrl, params);
}

quint32 ApiModel::put(const QUrl &url, const QVariantMap &put)
{
    Q_D(ApiModel);
    return d->request(ApiModel::METHOD_PUT, url, put);
}

quint32 ApiModel::put(const QVariantMap &params)
{
    Q_D(ApiModel);
    Q_ASSERT(!d->m_baseUrl.isEmpty());
    return d->request(ApiModel::METHOD_PUT, d->m_baseUrl, params);
}

quint32 ApiModel::del(const QUrl &url, const QVariantMap &params)
{
    Q_D(ApiModel);
    return d->request(ApiModel::METHOD_DELETE, url, params);
}

quint32 ApiModel::del(const QVariantMap &params)
{
    Q_D(ApiModel);
    Q_ASSERT(!d->m_baseUrl.isEmpty());
    return d->request(ApiModel::METHOD_DELETE, d->m_baseUrl, params);
}

void ApiModel::setQueryFormat(ApiModel::ApiDataFormat format)
{
    Q_D(ApiModel);
    if (d->m_queryFormatType != format) {
        d->m_queryFormatType = format;

        delete d->m_queryFormat;
        d->m_queryFormat = ApiModelFormatPrivate::factory(format);
    }
}

ApiModel::ApiDataFormat ApiModel::queryFormat() const
{
    Q_D(const ApiModel);
    return d->m_queryFormatType;
}

void ApiModel::setResponseFormat(ApiModel::ApiDataFormat format)
{
    Q_D(ApiModel);
    if (d->m_responseFormatType != format) {
        d->m_responseFormatType = format;

        delete d->m_responseFormat;
        d->m_responseFormat = ApiModelFormatPrivate::factory(format);
    }
}

ApiModel::ApiDataFormat ApiModel::responseFormat() const
{
    Q_D(const ApiModel);
    return d->m_responseFormatType;
}

void ApiModel::setResponseValidator(ApiModel::ApiResponseValidatorCallback validator)
{
    Q_D(ApiModel);
    d->m_responseValidatorCallback = validator;
}

void ApiModel::setResponseHeaderCallback(ApiModel::ApiResponseHeaderCallback callback)
{
    Q_D(ApiModel);
    d->m_responseHeaderCallback = callback;
}

void ApiModel::setCookies(const QList<QNetworkCookie> &cookies)
{
    Q_D(ApiModel);
    QNetworkCookieJar *cookieJar = new QNetworkCookieJar();

    foreach (QNetworkCookie cookie, cookies)
        cookieJar->insertCookie(cookie);

    d->m_networkAccessManager->setCookieJar(cookieJar);
    d->m_cookieJar = cookieJar;
}

void ApiModel::setBaseUrl(const QUrl &url)
{
    Q_D(ApiModel);
    if (d->m_baseUrl != url) {
        d->m_baseUrl = url;
        emit baseUrlChanged();
    }
}

QUrl ApiModel::baseUrl() const
{
    Q_D(const ApiModel);
    return d->m_baseUrl;
}

void ApiModel::setRestrictMode(bool restrictMode)
{
    Q_D(ApiModel);
    if (d->m_restrictMode != restrictMode) {
        d->m_restrictMode = restrictMode;
        emit restrictModeChanged();
    }
}

bool ApiModel::restrictMode() const
{
    Q_D(const ApiModel);
    return d->m_restrictMode;
}

bool ApiModel::availableForRequest() const
{
    Q_D(const ApiModel);
    const bool available = !d->m_restrictMode || !d->m_currentReply || d->m_currentReply->isFinished();

    return available;
}

void ApiModel::setRoutes(const QVariantList &routes)
{
    Q_D(ApiModel);

    d->m_routes.clear();

    int i = 0;
    int len = routes.count();

    for (; i < len; i++) {
        if (routes.at(i).type() != QVariant::Map) {
            qCWarning(LApiModel) << "Invalid route info.";
            continue;
        }

        QVariantMap routeInfo;
        QString route;
        QVariantMap info;

        routeInfo = routes.at(i).toMap();
        if (!routeInfo.contains(QLatin1String("route"))
                || routeInfo.value(QLatin1String("route")).type() != QVariant::String
                || routeInfo.value(QLatin1String("route")).toString().isEmpty()) {
            qCWarning(LApiModel) << "Route name is missing.";
            continue;
        }

        route = routeInfo.value(QLatin1String("route")).toString();
        if (d->m_routes.contains(route)) {
            qCWarning(LApiModel) << "Duplicate route name: " << route;
            continue;
        }
        info.insert(QLatin1String("route"), route);

        ApiModel::ApiMethod method;
        if (!routeInfo.contains(QLatin1String("method"))
                || routeInfo.value(QLatin1String("method")).type() != QVariant::Int
                || routeInfo.value(QLatin1String("method")).toInt() < ApiModel::METHOD_GET
                || routeInfo.value(QLatin1String("method")).toInt() > ApiModel::METHOD_DELETE) {
            method = ApiModel::METHOD_GET;
        } else {
            method = static_cast<ApiModel::ApiMethod>(routeInfo.value(QLatin1String("method")).toInt());
        }
        info.insert(QLatin1String("method"), method);

        if (routeInfo.contains(QLatin1String("url"))
                && routeInfo.value(QLatin1String("url")).type() == QVariant::String) {
            info.insert(QLatin1String("url"), routeInfo.value(QLatin1String("url")).toString());
        }

        if (routeInfo.contains(QLatin1String("defaults"))
                && routeInfo.value(QLatin1String("defaults")).type() == QVariant::Map) {
            info.insert(QLatin1String("defaults"), routeInfo.value(QLatin1String("defaults")).toMap());
        }

        if (routeInfo.contains(QLatin1String("requires"))
                && routeInfo.value(QLatin1String("requires")).type() == QVariant::String) {
            info.insert(QLatin1String("requires"), routeInfo.value(QLatin1String("requires")).toString().split(','));
        }

        d->m_routes.insert(info.value(QLatin1String("route")).toString(), info);
    }
}

void ApiModel::init()
{
    Q_D(ApiModel);
    d->init();
}

void ApiModel::reset()
{
    setRoutes(QVariantList());
    setRestrictMode(false);
    setBaseUrl(QUrl());
}

const char *ApiModel::name()
{
    return NAME;
}

void ApiModel::apply(const QVariantMap &config)
{
    if (config.contains(QLatin1String("routes")) && config.value(QLatin1String("routes")).type() == QVariant::List)
        setRoutes(config.value(QLatin1String("routes")).toList());

    if (config.contains(QLatin1String("restrictMode")) && config.value(QLatin1String("restrictMode")).type() == QVariant::Bool)
        setRestrictMode(config.value(QLatin1String("restrictMode")).toBool());

    if (config.contains(QLatin1String("baseUrl")) &&
            (config.value(QLatin1String("baseUrl")).type() == QVariant::String
             || config.value(QLatin1String("baseUrl")).type() == QVariant::Url))
        setBaseUrl(config.value(QLatin1String("baseUrl")).type() == QVariant::String ? config.value(QLatin1String("baseUrl")).toString() : config.value(QLatin1String("baseUrl")).toUrl());
}

void ApiModel::setConfig(const QVariantMap &config)
{
    apply(config);
}

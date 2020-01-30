#ifndef APIMODEL_H
#define APIMODEL_H

#include <QObject>
#include <QLoggingCategory>
#include <qqml.h>
#include <QNetworkReply>
#include <QNetworkCookie>
#include "gmvcmodel.h"
#include "gmvc_global.h"

Q_DECLARE_LOGGING_CATEGORY(LApiModel)

class ApiModel;
class ApiModelPrivate;
class QUrlQuery;

class Q_MVC_EXPORT ApiFormatError
{
public:
    enum FormatError {
        NoError,
        ParseError,
        AssambleError
    };

    ApiFormatError(FormatError error = NoError);

    FormatError error;
    QString errorString() const;
};

class Q_MVC_EXPORT ApiModel : public MvcModel
{
    Q_OBJECT
    Q_ENUMS(ApiError)
    Q_ENUMS(ApiMethod)
    Q_PROPERTY(QUrl baseUrl READ baseUrl WRITE setBaseUrl NOTIFY baseUrlChanged)
    Q_PROPERTY(bool restrictMode READ restrictMode WRITE setRestrictMode NOTIFY restrictModeChanged)
public:
    enum ApiDataFormat {
        FORMAT_QUERY = 0,
        FORMAT_JSON,
        FORMAT_JSONP
    };

    enum ApiError {
        NoError = 0,
        CouldNotConnectToHost,
        UnexpectedServerResponse,
        ShouldWaitUntilCurrentProcessIsCompleted
    };

    enum ApiMethod {
        METHOD_GET = 0,
        METHOD_POST,
        METHOD_PUT,
        METHOD_DELETE
    };

    typedef bool (*ApiResponseHeaderCallback)(QNetworkReply *reply, ApiModel *apiModel, const QString &identifier);
    typedef bool (*ApiResponseValidatorCallback)(const QVariant &data, ApiModel *apiModel, bool *errorControlOverride);

    static const char *NAME;

    explicit ApiModel(MvcFacade *parent = 0);
    ~ApiModel();

    Q_INVOKABLE quint32 api(const QString &route, const QVariantMap &data = QVariantMap());
    Q_INVOKABLE quint32 request(ApiMethod method, const QUrl &url, const QVariantMap &data = QVariantMap(), const QString &identifier = QString());
    Q_INVOKABLE quint32 post(const QUrl &url, const QVariantMap &post);
    Q_INVOKABLE quint32 post(const QVariantMap &post);
    Q_INVOKABLE quint32 get(const QUrl &url, const QVariantMap &params);
    Q_INVOKABLE quint32 get(const QVariantMap &params);
    Q_INVOKABLE quint32 put(const QUrl &url, const QVariantMap &put);
    Q_INVOKABLE quint32 put(const QVariantMap &params);
    Q_INVOKABLE quint32 del(const QUrl &url, const QVariantMap &params);
    Q_INVOKABLE quint32 del(const QVariantMap &params);

    Q_INVOKABLE bool availableForRequest() const;

    void setQueryFormat(ApiModel::ApiDataFormat format);
    ApiModel::ApiDataFormat queryFormat() const;

    void setResponseFormat(ApiModel::ApiDataFormat format);
    ApiModel::ApiDataFormat responseFormat() const;

    void setResponseValidator(ApiResponseValidatorCallback validator);
    void setResponseHeaderCallback(ApiResponseHeaderCallback callback);

    void setCookies(const QList<QNetworkCookie> &cookies);

    void setBaseUrl(const QUrl &url);
    QUrl baseUrl() const;

    Q_INVOKABLE void setRoutes(const QVariantList &routes);

    void setRestrictMode(bool restrict);
    bool restrictMode() const;

    Q_INVOKABLE void reset();

    void init();
    const char *name();

    void apply(const QVariantMap &config);

    Q_INVOKABLE void setConfig(const QVariantMap &config);

Q_SIGNALS:
    void baseUrlChanged();
    void cookiesChanged();
    void restrictModeChanged();
    void response(const QUrl &url, const QVariant &data, const QString &identifier, quint32 requestId);
    void error(ApiError error, const QString &errorString, const QString &identifier, quint32 requestId);

private:
    Q_DECLARE_PRIVATE(ApiModel)
    ApiModelPrivate *d_ptr;
};

QML_DECLARE_TYPE(ApiModel)

Q_DECLARE_METATYPE(ApiModel::ApiError)
Q_DECLARE_METATYPE(ApiModel::ApiMethod)

#endif // APIMODEL_H

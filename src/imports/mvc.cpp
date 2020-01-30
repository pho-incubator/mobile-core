#include <QtQml/qqmlextensionplugin.h>
#include <QtQml/qqml.h>

#include <GroupsIncMvc/gmvcfacade.h>
#include <GroupsIncMvc/gscreenmodel.h>
#include <GroupsIncMvc/gassetmodel.h>
#include <GroupsIncMvc/gapimodel.h>

QT_BEGIN_NAMESPACE

GMVC_DEFINE_MODEL(ScreenModel)
GMVC_DEFINE_MODEL(AssetModel)
GMVC_DEFINE_MODEL(ApiModel)

static inline QObject *getMvcFacade(QQmlEngine *, QJSEngine *)
{
    return MvcFacade::instance();
}

class GMvcModule : public QQmlExtensionPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QQmlExtensionInterface/1.0")
public:
    void registerTypes(const char *uri)
    {
        Q_ASSERT(QLatin1String(uri) == QLatin1String("GroupsIncMvc"));

        // @uri GroupsIncMvc
        qmlRegisterSingletonType<MvcFacade>(uri, 1, 0, "MvcFacade", getMvcFacade);
        qmlRegisterSingletonType<ScreenModel>(uri, 1, 0, "ScreenModel", GMVC_MODEL(ScreenModel));
        qmlRegisterSingletonType<AssetModel>(uri, 1, 0, "AssetModel", GMVC_MODEL(AssetModel));
        qmlRegisterSingletonType<ApiModel>(uri, 1, 0, "ApiModel", GMVC_MODEL(ApiModel));
    }

    void initializeEngine(QQmlEngine *engine, const char *uri)
    {
        Q_UNUSED(uri);
        Q_UNUSED(engine);
    }
};

QT_END_NAMESPACE

#include "mvc.moc"




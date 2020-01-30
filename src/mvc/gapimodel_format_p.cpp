#include "gapimodel_format_p.h"
#include "gapimodel_format_json_p.h"
#include "gapimodel_format_jsonp_p.h"
#include "gapimodel_format_query_p.h"

ApiModelFormatPrivate::ApiModelFormatPrivate()
    : QObject(0)
{
}

ApiModelFormatPrivate *ApiModelFormatPrivate::factory(ApiModel::ApiDataFormat format)
{
    ApiModelFormatPrivate *model = 0;
    switch (format) {
    case ApiModel::FORMAT_JSON:
        model = new ApiModelFormatJsonPrivate();
        break;
    case ApiModel::FORMAT_JSONP:
        model = new ApiModelFormatJsonPPrivate();
        break;
    // case ApiModel::FORMAT_QUERY:
    default:
        model = new ApiModelFormatQueryPrivate();
        break;
    }

    return model;
}

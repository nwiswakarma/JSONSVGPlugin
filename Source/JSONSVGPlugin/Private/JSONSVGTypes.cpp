// 

#pragma once

#include "JSONSVGTypes.h"
#include "JSONSVGLogs.h"
#include "Serialization/JsonTypes.h"
#include "Serialization/JsonReader.h"
#include "Serialization/JsonSerializer.h"

FJSONSVGDoc UJSONSVGUtils::ExtractSVG(const FString& JSONString)
{
    FJSONSVGDoc svg;

    TPSJSONObject& svgJSONObj( svg.JSONObject );
    TSharedRef< TJsonReader<> > JsonReader = TJsonReaderFactory<>::Create(JSONString);

    // Deserialize json string into svg doc object
    if (!FJsonSerializer::Deserialize(JsonReader, svgJSONObj) || !svgJSONObj.IsValid())
    {
        UE_LOG(LogJson, Warning, TEXT("JsonObjectStringToUStruct - Unable to parse json=[%s]"), *JSONString);
        return svg;
    }
    if (!FJsonObjectConverter::JsonObjectToUStruct(svgJSONObj.ToSharedRef(), &svg, 0, 0))
    {
        UE_LOG(LogJson, Warning, TEXT("JsonObjectStringToUStruct - Unable to deserialize. json=[%s]"), *JSONString);
        return svg;
    }

    UE_LOG(UntJSONSVG,Warning, TEXT("svg.Name: %s"), *svg.Name.ToString());
    UE_LOG(UntJSONSVG,Warning, TEXT("svg.Title: %s"), *svg.Title);

    FJSONSVGAttr& svgAttr( svg.Attrs );

    UE_LOG(UntJSONSVG,Warning, TEXT("\tsvg.Attrs.Width: %d"), svgAttr.Width);
    UE_LOG(UntJSONSVG,Warning, TEXT("\tsvg.Attrs.Height: %d"), svgAttr.Height);

    FJSONSVGViewBox& viewbox( svgAttr.ViewBox );

    UE_LOG(UntJSONSVG,Warning, TEXT("\tsvg.Attrs.ViewBox: [%d %d %d %d]"),
        viewbox.x, viewbox.y, viewbox.w, viewbox.h);

    svg.SetValid(true);
    ExtractChildren(svg);

    return MoveTemp(svg);
}

void UJSONSVGUtils::ExtractChildren(FJSONSVGElement& Element)
{
    if (! Element.JSONObject.IsValid())
    {
        UE_LOG(LogJSONSVG,Warning, TEXT("UJSONSVGUtils::ExtractChildren() ABORTED, Invalid element"));
        return;
    }

    TPSJSONObject& elmJSONObj( Element.JSONObject );
    const TArray<TPSJSONValue>* pChildValues = nullptr;

    if (! elmJSONObj->TryGetArrayField("childs", pChildValues))
    {
        return;
    }

    check(pChildValues);

    Element.Children.Reserve(pChildValues->Num());

    const TArray<TPSJSONValue>& childValues( *pChildValues );
    for (const TPSJSONValue& childValue : childValues)
    {
        const TPSJSONObject* pChildObj = nullptr;
        if (childValue->TryGetObject(pChildObj))
        {
            Element.Children.Emplace(*pChildObj);
        }
    }

    Element.Children.Shrink();

    for (FJSONSVGElement& child : Element.Children)
    {
        UE_LOG(UntJSONSVG,Warning, TEXT("Element [%s] Child: %s"), *Element.Name.ToString(), *child.Name.ToString());

        child.SetValid(true);
        ExtractChildren(child);
    }
}

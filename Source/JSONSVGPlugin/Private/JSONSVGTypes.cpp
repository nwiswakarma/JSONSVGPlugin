////////////////////////////////////////////////////////////////////////////////
//
// MIT License
// 
// Copyright (c) 2018-2019 Nuraga Wiswakarma
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
//
////////////////////////////////////////////////////////////////////////////////
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

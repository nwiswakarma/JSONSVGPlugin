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

#include "CoreMinimal.h"
#include "CoreUObject.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Dom/JsonObject.h"
#include "JsonObjectConverter.h"
#include "JSONSVGTypes.generated.h"

typedef TSharedPtr<FJsonValue> TPSJSONValue;
typedef TSharedPtr<FJsonObject> TPSJSONObject;

USTRUCT(BlueprintType)
struct JSONSVGPLUGIN_API FJSONSVGElement
{
	GENERATED_BODY()

    FJSONSVGElement() = default;

    FJSONSVGElement(const TPSJSONObject& InJSONObject) : JSONObject(InJSONObject)
    {
        if (JSONObject.IsValid())
        {
            FString nameStr;
            JSONObject->TryGetStringField("name", nameStr);
            Name = FName(*nameStr);
        }
    }

    FORCEINLINE bool IsValid() const
    {
        return bIsValid;
    }

    FORCEINLINE void SetValid(bool bInIsValid)
    {
        bIsValid = bInIsValid;
    }

    template<typename ElementType>
    FORCEINLINE bool AsType(ElementType& OutElement) const
    {
        if (JSONObject.IsValid() && FJsonObjectConverter::JsonObjectToUStruct(JSONObject.ToSharedRef(), &OutElement, 0, 0))
        {
            OutElement.SetValid(true);
        }
        return OutElement.IsValid();
    }

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
    FName Name;

    TPSJSONObject JSONObject;
    TArray<FJSONSVGElement> Children;

protected:

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
    bool bIsValid = false;
};

USTRUCT(BlueprintType)
struct JSONSVGPLUGIN_API FJSONSVGShapeAttr
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
    FString Fill = "#000000";

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
    FString Stroke = "#000000";

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
    float StrokeWidth = 0.f;

    //FJSONSVGShapeAttr& operator=(const FJSONSVGShapeAttr& rhs)
    //{
    //    Fill = rhs.Fill;
    //    Stroke = rhs.Stroke;
    //    StrokeWidth = rhs.StrokeWidth;
    //    return *this;
    //}
};

USTRUCT(BlueprintType)
struct JSONSVGPLUGIN_API FJSONSVGPathData
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
    uint8 code = 0;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
    float x0 = 0.f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
    float y0 = 0.f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
    float x1 = 0.f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
    float y1 = 0.f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
    float x2 = 0.f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
    float y2 = 0.f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
    float x = 0.f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
    float y = 0.f;
};

USTRUCT(BlueprintType)
struct JSONSVGPLUGIN_API FJSONSVGPathAttr : public FJSONSVGShapeAttr
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
    TArray<FJSONSVGPathData> D;
};

USTRUCT(BlueprintType)
struct JSONSVGPLUGIN_API FJSONSVGPath : public FJSONSVGElement
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
    FJSONSVGPathAttr Attrs;
};

USTRUCT(BlueprintType)
struct JSONSVGPLUGIN_API FJSONSVGCircleAttr : public FJSONSVGShapeAttr
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
    float cx = 0.f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
    float cy = 0.f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
    float r = 0.f;
};

USTRUCT(BlueprintType)
struct JSONSVGPLUGIN_API FJSONSVGCircle : public FJSONSVGElement
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
    FJSONSVGCircleAttr Attrs;
};

USTRUCT(BlueprintType)
struct JSONSVGPLUGIN_API FJSONSVGEllipseAttr : public FJSONSVGShapeAttr
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
    float cx = 0.f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
    float cy = 0.f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
    float rx = 0.f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
    float ry = 0.f;
};

USTRUCT(BlueprintType)
struct JSONSVGPLUGIN_API FJSONSVGEllipse : public FJSONSVGElement
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
    FJSONSVGEllipseAttr Attrs;
};

USTRUCT(BlueprintType)
struct JSONSVGPLUGIN_API FJSONSVGRectAttr : public FJSONSVGShapeAttr
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
    float x = 0.f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
    float y = 0.f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
    float rx = 0.f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
    float ry = 0.f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
    float Width = 0.f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
    float Height = 0.f;
};

USTRUCT(BlueprintType)
struct JSONSVGPLUGIN_API FJSONSVGRect : public FJSONSVGElement
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
    FJSONSVGRectAttr Attrs;
};

USTRUCT(BlueprintType)
struct JSONSVGPLUGIN_API FJSONSVGViewBox
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
    int32 x = 0;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
    int32 y = 0;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
    int32 w = 0;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
    int32 h = 0;
};

USTRUCT(BlueprintType)
struct JSONSVGPLUGIN_API FJSONSVGAttr
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
    FJSONSVGViewBox ViewBox;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
    int32 Width = 0;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
    int32 Height = 0;
};

USTRUCT(BlueprintType)
struct JSONSVGPLUGIN_API FJSONSVGDoc : public FJSONSVGElement
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
    FString Title = "Unnamed SVG";

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
    FJSONSVGAttr Attrs;
};

UCLASS(BlueprintType, Blueprintable)
class JSONSVGPLUGIN_API UJSONSVGUtils : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

    UFUNCTION(BlueprintCallable, Category = "JSON SVG")
    static FJSONSVGDoc ExtractSVG(const FString& JSONString);

private:

    static void ExtractChildren(FJSONSVGElement& Element);

};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "any"
#include "Engine/StaticMeshActor.h"
#include "Containers/Array.h"
#include "Delegates/Delegate.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "ConditionLibrary.generated.h"

//template<class R, class...Args>
//struct Custom_func;
//template<class R, class...Args>
//struct Custom_func<R(Args...)> {
//	
//	mutable std::any State;
//	R(*F)(std::any& State, Args&&...) = nullptr;
//	
//	template<class T>
//	void Bind(T&& Type) {
//		State = std::forward<T>(Type);
//		F = [](std::any& state, Args&&...args)->R {
//			return std::any_cast<T&>(state)(std::forward<Args>(args)...);
//		};
//	}
//	
//	R operator()(Args...Arguments)const {
//		return F(State, std::forward<Args>(Arguments)...);
//	}
//};

//template<typename ...Args>
//class TCondChecker
//{
//	
//	TTuple<TFunction<bool(Args...)>> Functors;
//public:
//	TCondChecker() = delete;
//
//	template<typename ...Funcs>
//	TCondChecker(Funcs... funcs) :Functors(MakeTuple(funcs...)) {}
//
//	bool operator()(Args... args)
//	{
//		return CheckConditions(std::index_sequence_for<Args...>(), args...);
//	}
//
//private:
//	
//	template<size_t... Is>
//	bool CheckConditions(std::index_sequence<Is...>, Args... args)
//	{
//		return std::min({std::get<Is>(Functors)(args...)...});
//	}
//};
template<typename ...Args>
class TCondChecker
{
private:
	TArray<TFunction<bool(Args...)>> Functors;

public:
	TCondChecker() = delete;

	template<typename ...Funcs>
	TCondChecker( const Funcs&&... funcs) : Functors({ Forward<Funcs> (funcs)... }) {}
	
	std::pair<bool,TArray<bool>> operator()(Args... args)
	{
		return CheckConditions(std::index_sequence_for<Args...>(), Forward<Args>(args)...);
	}

private:

	template<size_t... Is>
	std::pair<bool,TArray<bool>> CheckConditions(std::index_sequence<Is...>, Args... args)
	{
		const auto Result ={ Functors[Is](Forward<Args>(args)...)... };
		return std::make_pair<bool,TArray<bool>>(std::min(Result), Result);
	}
};


UCLASS()
class HERRAMIENTAS_API UConditionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
#if WITH_EDITOR
	UFUNCTION(BlueprintCallable)
	static bool CheckTriCount(const UStaticMesh* StaticMesh);
	UFUNCTION(BlueprintCallable)
	static bool CheckMaterialSlots(const UStaticMesh* StaticMesh);
	UFUNCTION(BlueprintCallable)
	static bool CheckAbsoluteSpace(const UStaticMesh* StaticMesh);
	UFUNCTION(BlueprintCallable)
	static void CheckConditionsForStaticMeshes(TArray<FString>& OutMeshesName, TArray<AStaticMeshActor*> InStaticMeshes);
	UFUNCTION(BlueprintCallable)
	static bool ExportStaticMeshes(const TArray<FString>& PackageNames);


#endif
	
	
};

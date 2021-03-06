// Fornwest ©2021 itsdits. All Rights Reserved.

#include "CharacterTest.h"
#include "Fornwest/FornwestCharacter.h"
#include "Misc/AutomationTest.h"
#include "Editor.h"

#if WITH_DEV_AUTOMATION_TESTS

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FAFornwestCharacterShouldntBeNullWhenInstantiatedTest, "Game.Unit.FornwestCharacterTests.ShouldntBeNullWhenInstantiated", EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter)

bool FAFornwestCharacterShouldntBeNullWhenInstantiatedTest::RunTest(const FString& Parameters)
{
	{
		AFornwestCharacter* TestCharacter = NewObject<AFornwestCharacter>();

		TestNotNull(TEXT("The character shouldn't be null after instantiating it."), TestCharacter);
	}

	return true;
}

#endif //WITH_DEV_AUTOMATION_TESTS
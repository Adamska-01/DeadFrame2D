#pragma once


#define TYPE_INFO(Type, ParentType) \
public: \
	static inline const ::DeadFrame2D::Core::TypeInfo StaticTypeInfo{ #Type, &ParentType::StaticTypeInfo }; \
	inline virtual const ::DeadFrame2D::Core::TypeInfo* GetTypeInfo() const override { return &StaticTypeInfo; } \
private:


namespace DeadFrame2D::Core
{
	struct TypeInfo
	{
		// This is not really used, but nice to have
		const char* name;

		const TypeInfo* parent;
	};
}
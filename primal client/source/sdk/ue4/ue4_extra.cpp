#include "ue4_extra.h"

std::string UObject::get_name() const
{
	std::string name(Name.get_name());
	if (Name.Number > 0)
	{
		name += '_' + std::to_string(Name.Number);
	}
	auto pos = name.rfind('/');
	if (pos == std::string::npos)
	{
		return name;
	}
	return name.substr(pos + 1);
}

std::string UObject::get_full_name() const
{
	std::string name;
	if (Class != nullptr)
	{
		std::string temp;
		for (auto p = Outer; p; p = p->Outer)
			temp = p->get_name() + "." + temp;

		name = Class->get_name();
		name += " ";
		name += temp;
		name += get_name();
	}
	return name;
}

bool UObject::instance_of(UClass* cmp) const
{
	for (auto super = Class; super; super = static_cast<UClass*>(super->SuperField))
	{
		if (super == cmp)
		{
			return true;
		}
	}
	return false;
}
#ifndef _XCHIP_MEDIA_UNIQUEMEDIAPLUGIN_H_
#define _XCHIP_MEDIA_UNIQUEMEDIAPLUGIN_H_
#include <XChip/Utility/BaseTraits.h>
#include <XChip/Utility/Memory.h>
#include "iMediaPlugin.h"

namespace xchip {

template<class T>
class UniqueMediaPlugin : public std::unique_ptr<T, MediaPluginDeleter>
{
	static_assert(utility::is_same<T, iMediaPlugin>::value || utility::is_same<T, iRender>::value ||
                      utility::is_same<T, iInput>::value || utility::is_same<T, iSound>::value,
                      "UniqueMediaPlugin must be a iMediaPlugin");
public:

	UniqueMediaPlugin()
		: std::unique_ptr<T, MediaPluginDeleter>(nullptr, delete_media_plugin)
	{
	}

	template<class ...Args>
	UniqueMediaPlugin(Args&& ...args) 
		: std::unique_ptr<T, MediaPluginDeleter>(std::forward<Args>(args)..., delete_media_plugin)
	{
	}

	
	UniqueMediaPlugin(std::unique_ptr<T>&& rhs)
		: std::unique_ptr<T, MediaPluginDeleter>(rhs.release(), delete_media_plugin)
	{
	}



private:
	static void delete_media_plugin(const iMediaPlugin* plugin) { plugin->GetPluginDeleter()(plugin); }


};












}

#endif

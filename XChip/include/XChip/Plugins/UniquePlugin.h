#ifndef _XCHIP_MEDIA_UNIQUEPLUGIN_H_
#define _XCHIP_MEDIA_UNIQUEPLUGIN_H_
#include <XChip/Utility/BaseTraits.h>
#include <XChip/Utility/Memory.h>
#include "iPlugin.h"

namespace xchip {

template<class T>
class UniquePlugin : public std::unique_ptr<T, PluginDeleter>
{
	static_assert(utility::is_same<T, iPlugin>::value || utility::is_same<T, iRender>::value ||
                      utility::is_same<T, iInput>::value || utility::is_same<T, iSound>::value,
                      "UniquePlugin must be a type of iPlugin interface");
public:

	UniquePlugin()
		: std::unique_ptr<T, PluginDeleter>(nullptr, delete_media_plugin)
	{
	}

	template<class ...Args>
	UniquePlugin(Args&& ...args) 
		: std::unique_ptr<T, PluginDeleter>(std::forward<Args>(args)..., delete_media_plugin)
	{
	}

	
	UniquePlugin(std::unique_ptr<T>&& rhs)
		: std::unique_ptr<T, PluginDeleter>(rhs.release(), delete_media_plugin)
	{
	}



private:
	static void delete_media_plugin(const iPlugin* plugin) { plugin->GetPluginDeleter()(plugin); }


};












}

#endif

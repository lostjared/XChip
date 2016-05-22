#ifndef _XCHIP_MEDIA_UNIQUEMEDIAPLUGIN_H_
#define _XCHIP_MEDIA_UNIQUEMEDIAPLUGIN_H_
#include <XChip/Utility/Memory.h>
#include "iMediaPlugin.h"

namespace xchip {

template<class T>
class UniqueMediaPlugin : public std::unique_ptr<T, MediaPluginDeleter>
{
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

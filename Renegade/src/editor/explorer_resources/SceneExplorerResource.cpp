#ifdef __EDITOR__

#include "editor/explorer_resources/SceneExplorerResource.h"

#include <rapidjson/utils.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/prettywriter.h>

#include "core/Engine.h"
#include "utils/string_extensions.h"
#include "core/datatypes/DataStream.h"
#include "file/FileLoader.h"
#include "gameplay/ECSBaseSystem.h"
#include "gameplay/systems/EntityDetailComponent.h"
#include "gameplay/systems/EntityDetailSystem.h"

#define JSON_SCENE_ENTITIES_VAR "entities"

namespace renegade
{
	namespace editor
	{
		void SceneExplorerResource::SetWindowTitle()
		{
			std::string windowTitle = (core::ENGINE.GetEditor().GetCurrentScene()->IsDirty() ? "*" : "") + string_extensions::GetFileName(core::ENGINE.GetEditor().GetCurrentScene()->GetPath());
			core::ENGINE.GetWindow().SetTitle(windowTitle);
		}

		void SceneExplorerResource::SetDirty()
		{
			ExplorerResource::SetDirty();
			SetWindowTitle();
		}

		void SceneExplorerResource::ResetDirty()
		{
			ExplorerResource::ResetDirty();
			SetWindowTitle();
		}

		bool SceneExplorerResource::Save()
		{
			rapidjson::Document document;
			document.SetObject();
			rapidjson::Document::AllocatorType& allocator = document.GetAllocator();

			document.AddMember(JSON_SCENE_ENTITIES_VAR, rapidjson::Value().SetArray(), allocator);
			for (gameplay::EntityID& id : core::ENGINE.GetECS().GetEntities())
			{
				rapidjson::Document idDocument;
				idDocument.SetObject();

				for (gameplay::ECSSystem* system : core::ENGINE.GetECS().GetSystemsContainingEntity(id))
				{
					rapidjson::Value key(system->GetPropertyName().c_str(), allocator);
					idDocument.AddMember(key, rapidjson::Value().SetObject(), allocator);
					gameplay::Component* component = system->GetBaseComponent(id);
					component->Serialize(idDocument[system->GetPropertyName().c_str()], allocator);
				}
				document[JSON_SCENE_ENTITIES_VAR].PushBack(idDocument, allocator);
			}

			rapidjson::StringBuffer buffer;
			rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(buffer);
			document.Accept(writer);

			std::string path = m_Path;
			std::promise<bool> promise;
			std::future<bool> future = promise.get_future();
			core::ENGINE.GetFileLoader().EnqueueTask([&path, &buffer]() mutable
			{
				return file::FileLoader::SaveFile(path, core::Data(buffer.GetString(), buffer.GetSize()));
			}, promise, future);

			if (future.get())
			{
				ResetDirty();
				return true;
			}
			LOGF(LOGSEVERITY_ERROR, "Something went wrong when trying to save scene file '%s'.", m_Path);
			return false;
		}

		bool SceneExplorerResource::Load()
		{
			rapidjson::Document document;
			core::DataStream data;
			if (!file::FileLoader::LoadFile(m_Path, data))
			{
				return false;
			}

			document.Parse(reinterpret_cast<char*>(data.data()), data.size());

			if (document.HasParseError())
			{
				LOGF(LOGSEVERITY_ERROR, "Something went wrong when trying to load scene file '%s'.", m_Path);
				return false;
			}

			core::ENGINE.GetECS().Clear();

			if (document.HasMember(JSON_SCENE_ENTITIES_VAR) && document[JSON_SCENE_ENTITIES_VAR].IsArray())
			{
				for (auto& element : document[JSON_SCENE_ENTITIES_VAR].GetArray())
				{
					if (!element.IsObject())
					{
						continue;
					}

					gameplay::EntityID id = core::ENGINE.GetECS().CreateEntity("");

					for (gameplay::ECSSystem* system : core::ENGINE.GetECS().GetSystems())
					{
						if (element.HasMember(system->GetPropertyName().c_str()))
						{
							gameplay::Component* component = system->GetBaseComponent(id);
							const rapidjson::Value& testMember = element[system->GetPropertyName().c_str()];
							component->Deserialize(testMember, document.GetAllocator());
						}
					}
				}
			}
		}
	}
}

#endif // __EDITOR__
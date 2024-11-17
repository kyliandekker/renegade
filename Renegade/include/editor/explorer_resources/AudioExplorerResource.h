#ifdef __EDITOR__

#pragma once

#include "editor/ExplorerResource.h"

namespace renegade
{
	namespace editor
	{
		class AudioExplorerResource : public ExplorerResource
		{
		public:
			virtual ~AudioExplorerResource() override;

			bool Initialize() override;
			double* m_LeftSamples = nullptr;
			double* m_RightSamples = nullptr;
			size_t m_NumSamples = 0;
		};

		class SoundExplorerResource : public AudioExplorerResource
		{
		public:
		};

		class SongExplorerResource : public AudioExplorerResource
		{
		public:
		};

		class VOExplorerResource : public AudioExplorerResource
		{
		public:
		};
	}
}

#endif // __EDITOR__
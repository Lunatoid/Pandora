#pragma once

#include "Pandora/Core/Types.h"
#include "Pandora/Core/Assert.h"
#include "Pandora/Core/VideoBackend.h"

#include "Pandora/Core/Time/Time.h"
#include "Pandora/Core/Time/Duration.h"
#include "Pandora/Core/Time/Stopwatch.h"

#include "Pandora/Core/Logging/PrintType.h"
#include "Pandora/Core/Logging/Logging.h"

#include "Pandora/Core/Data/Allocator.h"
#include "Pandora/Core/Data/Memory.h"
#include "Pandora/Core/Data/Slice.h"
#include "Pandora/Core/Data/Array.h"
#include "Pandora/Core/Data/StringView.h"
#include "Pandora/Core/Data/String.h"
#include "Pandora/Core/Data/Hash.h"
#include "Pandora/Core/Data/Dictionary.h"
#include "Pandora/Core/Data/Reference.h"
#include "Pandora/Core/Data/Pair.h"
#include "Pandora/Core/Data/Optional.h"

#include "Pandora/Core/Math/Math.h"
#include "Pandora/Core/Math/Lerp.h"
#include "Pandora/Core/Math/Random.h"
#include "Pandora/Core/Math/Vector.h"
#include "Pandora/Core/Math/Color.h"
#include "Pandora/Core/Math/Matrix.h"
#include "Pandora/Core/Math/Perlin.h"

#include "Pandora/Core/Encoding/Compression.h"
#include "Pandora/Core/Encoding/Encryption.h"
#include "Pandora/Core/Encoding/Base64.h"
#include "Pandora/Core/Encoding/JSON.h"
#include "Pandora/Core/Encoding/Box.h"

#if defined(PD_BOX_BUILDER)
#include "Pandora/Core/Encoding/BoxBuilder.h"
#endif

#include "Pandora/Core/IO/Stream.h"
#include "Pandora/Core/IO/FileStream.h"
#include "Pandora/Core/IO/MemoryStream.h"
#include "Pandora/Core/IO/Console.h"
#include "Pandora/Core/IO/Path.h"
#include "Pandora/Core/IO/File.h"
#include "Pandora/Core/IO/Folder.h"
#include "Pandora/Core/IO/Storage.h"

#include "Pandora/Core/Async/Atomics.h"
#include "Pandora/Core/Async/Thread.h"
#include "Pandora/Core/Async/Mutex.h"
#include "Pandora/Core/Async/Lock.h"
#include "Pandora/Core/Async/Condition.h"
#include "Pandora/Core/Async/Semaphore.h"

#include "Pandora/Core/Input/Key.h"
#include "Pandora/Core/Input/Input.h"
#include "Pandora/Core/Input/InputManager.h"

#include "Pandora/Core/Window/Window.h"
#include "Pandora/Core/Window/WindowEvent.h"

#include "Pandora/Core/Resources/ResourceType.h"
#include "Pandora/Core/Resources/Resource.h"
#include "Pandora/Core/Resources/BinaryResource.h"
#include "Pandora/Core/Resources/ResourceCatalog.h"

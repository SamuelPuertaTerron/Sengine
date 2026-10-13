#!/usr/bin/env bash
cd "$(dirname "${BASH_SOURCE[0]}")"
if [ -z "$SENGINE_ROOT" ]; then
	echo "SENGINE_ROOT is not set. Install the engine from the Sengine hub, or run Setup.sh in the engine folder."
	exit 1
fi
"$SENGINE_ROOT/ThirdParty/premake/premake5" gmake2

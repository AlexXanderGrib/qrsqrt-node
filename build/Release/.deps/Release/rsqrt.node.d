cmd_Release/rsqrt.node := ln -f "Release/obj.target/rsqrt.node" "Release/rsqrt.node" 2>/dev/null || (rm -rf "Release/rsqrt.node" && cp -af "Release/obj.target/rsqrt.node" "Release/rsqrt.node")

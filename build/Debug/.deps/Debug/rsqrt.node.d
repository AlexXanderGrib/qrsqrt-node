cmd_Debug/rsqrt.node := ln -f "Debug/obj.target/rsqrt.node" "Debug/rsqrt.node" 2>/dev/null || (rm -rf "Debug/rsqrt.node" && cp -af "Debug/obj.target/rsqrt.node" "Debug/rsqrt.node")

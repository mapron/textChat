import qbs;

Project {
  Product {
     name: "lib"
     Depends { name: "cpp"}
     Depends { name: 'Qt'; submodules: ['core', 'gui', 'widgets', 'network'] }
     type: 'staticlibrary';
     files: ['lib/*.cpp', 'lib/*.h'];
     cpp.includePaths: ['lib/']
     Export {
       Depends { name: 'cpp' }
       Depends { name: 'Qt'; submodules: ['core', 'gui', 'widgets', 'network'] }
       cpp.includePaths: ['lib/']
    }
  }
  
	CppApplication {
    Depends { name: 'lib' }
		files: ['client/*.cpp', 'client/*.h'];
		cpp.includePaths: ['client/']
		name: "chat-client"

	}

   CppApplication {
    Depends { name: 'lib' }
		files: ['server/*.cpp', 'server/*.h'];
		cpp.includePaths: ['server/']
		name: "chat-server"
	}

   CppApplication {
        Depends { name: 'lib' }
        files: ['tests/*.cpp', 'tests/*.h'];
        cpp.includePaths: ['tests/']
        name: "tests"
    }

}



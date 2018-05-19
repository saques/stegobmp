# Build en Windows:
1. ```git clone https://github.com/saques/stegobmp.git``` clonar el repositorio
2. ```git submodule init``` inicializar los submódulos
3. ```git submodule update```
4. Instalar [OpenSSL](https://slproweb.com/products/Win32OpenSSL.html), [Visual Studio 2017 Community](https://www.visualstudio.com/downloads/) y [CMake](https://cmake.org/download/).
5. Abrir el proyecto. File->Open->CMake...
7. Editar los includePaths del archivo CppProperties.json (Project->Edit Settings) para x64-Debug y x64-Release con:
```
"includePath": [
	"${env.INCLUDE}",
	"C:\\msys64\\mingw64\\include",
	"${projectRoot}\\cxxopts\\include",
	"${projectRoot}\\easyloggingpp\\src",
	"${projectRoot}"
],
```
6. Compilar y ejecutar.

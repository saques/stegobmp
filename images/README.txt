lado.bmp se usa como portador en todos los casos
ladoLSB1.bmp tiene oculto un archivo pdf, con m�todo LSB1 (sin encripci�n)
ladoLSB4.bmp tiene oculto un archivo pdf, con m�todo LSB4 (sin encripci�n)
ladoLSB4aes256ofb.bmp tiene oculto un archivo pdf, con m�todo LSB4, encriptado previamente con aes256, modo ofb, password "secreto"
ladoLBE.bmp tiene oculto un archivo png, con LSBE (sin encripci�n)

whiteOutLSB4.bmp, esteganografiado con los siguientes parametros:

{
  "version": "0.2.1",
  "defaults": {},
  "configurations": [
    {
      "type": "default",
      "project": "CMakeLists.txt",
      "projectTarget": "stegobmp.exe",
      "name": "stegobmp.exe",
      "args": [
        "--embed",
        "-p",
        "D:\\git\\stegobmp\\images\\white.bmp",
        "--out",
        "D:\\git\\stegobmp\\images\\whiteOutLSB4.bmp",
        "--in",
        "D:\\git\\stegobmp\\images\\lado.bmp",
        "-m",
        "ecb",
        "-a",
        "des",
        "--pass",
        "camaleon",
        "--steg",
        "LSB4"
      ]
    }
  ]
}

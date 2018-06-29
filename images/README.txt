lado.bmp se usa como portador en todos los casos
ladoLSB1.bmp tiene oculto un archivo pdf, con método LSB1 (sin encripción)
ladoLSB4.bmp tiene oculto un archivo pdf, con método LSB4 (sin encripción)
ladoLSB4aes256ofb.bmp tiene oculto un archivo pdf, con método LSB4, encriptado previamente con aes256, modo ofb, password "secreto"
ladoLBE.bmp tiene oculto un archivo png, con LSBE (sin encripción)

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

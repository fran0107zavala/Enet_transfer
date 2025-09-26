from Crypto.Cipher import AES
from scapy.all import sniff, sendp, Ether
import binascii

AES_KEY = b'clavedeDavid2025'
MAC_SCR = bytes.fromhex("d4bed9452260")  # Dirección MAC origen esperada
frases = ["...Ni todos los que vagan están perdidos.",
            "...brilla una luz." ,
          "Nada mas que un breve caminar a la luz del sol.",
          "...pues en ella se esconden las estrellas.",
          "...pueden cambiar el curso del futuro.",
          "...si aun te queda la luna.",
          "...en los lugares más inesperados.",
          "...son oro y plata.",
          "...cruzar tu puerta.",
        "...ni pronto, Frodo Bolsón. Llega precisamente cuando se lo propone." ,
"...mientras la Companía permanezca fiel.",
          "...Siento que algo se avecina.",
"...no alcanzan las heladas." ,
"...pasar." ,
          "...cuidado con el Anillo.",
          "...se despertara." ]
contador_respuesta = 0


def decrypt_payload(data, key):
    cipher = AES.new(key, AES.MODE_ECB)
    return cipher.decrypt(data)


def construir_respuesta(payload_original):
    global contador_respuesta
    frase_actual = frases[contador_respuesta]

    contador_respuesta = contador_respuesta + 1

    respuesta = Ether(dst=payload_original[6:12], src=payload_original[0:6]) / frase_actual

    sendp(respuesta, iface="\\Device\\NPF_{EB47B715-F099-487D-9D91-FEB7C6AFA0BF}", verbose=False)
    print(f"✅ Respuesta enviada ({contador_respuesta - 1}): {frases[contador_respuesta - 1]}")



def detectar(pkt):
    raw_bytes = bytes(pkt)

    # ⚠️ Asegurar que el paquete tenga al menos la estructura mínima esperada
    if len(raw_bytes) < 18:  # Mínimo: 14 header + 2 len + 2 crc
        print("⚠️ Paquete demasiado corto. Ignorado.")
        return

    index = raw_bytes.find(MAC_SCR)
    if index == -1:
        return  # No es el paquete que esperamos

    payload = raw_bytes

    datos_encrip = payload[12:14]
    padded_length = int.from_bytes(datos_encrip, 'big')
    datos = payload[14:-4]
    CRC = payload[-4:]
    datos1 = payload[14:14 + padded_length]

    crc_calculado = binascii.crc32(datos).to_bytes(4, byteorder='big')

    # 👉 Salir en silencio si el CRC no es válido
    if CRC != crc_calculado:
        return

    # ✅ Solo se muestra e interpreta si el CRC es válido
    print("\n✅ CRC válido, paquete aceptado.")

    if len(datos1) % 16 != 0:
        print(f"⚠️ Longitud del payload cifrado no es múltiplo de 16 ({len(datos1)} bytes), no se puede descifrar.")
        return

    decrypted = decrypt_payload(datos1, AES_KEY)

    # Eliminar padding (PKCS7)
    padding_len = decrypted[-1]
    if 1 <= padding_len <= 16:
        decrypted = decrypted[:-padding_len]

    try:
        decoded = decrypted.decode('utf-8')
        print(f"🔓 Payload descifrado (ascii): {decoded}")
        construir_respuesta(payload)
    except UnicodeDecodeError:
        print("⚠️ Payload no es texto UTF-8 legible.")





sniff(
    iface="\\Device\\NPF_{EB47B715-F099-487D-9D91-FEB7C6AFA0BF}",
    prn=detectar,
    store=0,
    promisc=True
)

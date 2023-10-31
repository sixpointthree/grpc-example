#!/bin/bash

# Protobuf-Datei und Ausgabeordner
PROTO_FILE="$1"
PROTO_FILE_PATH="$2"
OUTPUT_DIR="$3"

# Pfade zu den Protobuf- und gRPC-Tools
PROTOC_EXECUTABLE="$4"
GRPC_CPP_PLUGIN_EXECUTABLE="$5"

# Alle Parameter ausgeben
echo "Protobuf-Datei: $PROTO_FILE"
echo "Protobuf-Datei-Pfad: $PROTO_FILE_PATH"
echo "Ausgabeordner: $OUTPUT_DIR"
echo "Protobuf-Compiler: $PROTOC_EXECUTABLE"
echo "gRPC-Compiler: $GRPC_CPP_PLUGIN_EXECUTABLE"

# Prüfen, ob die Protobuf-Datei vorhanden ist
if [ ! -f "$PROTO_FILE" ]; then
  echo "Fehler: Die Protobuf-Datei $PROTO_FILE existiert nicht."
  exit 1
fi

# Erstellen des Ausgabeordners, falls er nicht existiert
mkdir -p "$OUTPUT_DIR"

# Generieren der Protobuf-Dateien
"$PROTOC_EXECUTABLE" --grpc_out="$OUTPUT_DIR" --cpp_out="$OUTPUT_DIR" --proto_path="$PROTO_FILE_PATH" --plugin=protoc-gen-grpc="$GRPC_CPP_PLUGIN_EXECUTABLE" "$PROTO_FILE"

ERRORCODE=$?

# Überprüfen, ob das Protobuf-Generieren erfolgreich war
if [ $ERRORCODE -ne 0 ]; then
  echo "Fehler: Das Generieren der Protobuf-Dateien ist fehlgeschlagen. Fehlercode: $ERRORCODE"
  exit 1
fi

# Überprüfen, ob das gRPC-Generieren erfolgreich war
if [ $? -ne 0 ]; then
  echo "Fehler: Das Generieren der gRPC-Dateien ist fehlgeschlagen. Fehlercode: $?"
  exit 1
fi

# Erfolgreiche Ausführung
echo "Protobuf und gRPC-Generierung erfolgreich abgeschlossen."
exit 0

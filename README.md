# miner_sha256d (C++)

Implémentation modulaire d'un mineur SHA256d en C++20 avec socle Stratum V2 (framing/session), moteur SHA256/SHA256d, gestion des jobs et scheduler multi-threads CPU.

## Build

```bash
cmake -S . -B build
cmake --build build -j
ctest --test-dir build --output-on-failure
```

## Exécution

```bash
./build/miner --config=config.json --set=perf.cpu_threads=4 --set=log.level=debug
```

## Arborescence

- `src/core`: config, logging structuré, métriques, runtime/backoff.
- `src/sv2`: framing, codec minimal messages, machine d'état session.
- `src/work`: header Bitcoin, merkle root, target compact (`nbits`).
- `src/hash`: SHA-256, SHA256d, utilitaires hex.
- `src/mining`: scheduler/worker CPU + file de shares.
- `src/tests`: vecteurs SHA-256 et checks target.

## Couverture fonctionnelle

- Config JSON + overrides CLI `--set=cle=valeur`.
- Profils pools + paramètres perf + paramètres SV2.
- Logger JSON avec rotation.
- Compteurs de base exportables en format Prometheus text.
- Framing SV2 (encode/decode longueur + type + payload).
- État de session (connect/setup/channel/mining).
- SHA256 complet + SHA256d + sérialisation header 80 bytes.
- Boucle de scan nonce multi-thread (CPU demo).

> Note: cette base est prête pour intégrer le transport TCP/TLS/Noise complet SV2 et le submit/ack réseau réel.

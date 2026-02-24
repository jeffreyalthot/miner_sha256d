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

## Configuration rapide pool en ligne de commande

Vous pouvez maintenant surcharger directement la config pool sans éditer `config.json` :

```bash
./build/miner \
  --pool-name=demo-pool \
  --pool-host=127.0.0.1 \
  --pool-port=3336 \
  --pool-user=wallet.worker01 \
  --pool-pass=x \
  --pool-tls=false \
  --set=sv2.device_id=rig-cpu-01 \
  --set=perf.cpu_threads=8
```

Aide intégrée :

```bash
./build/miner --help
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
- Raccourcis CLI dédiés au pool (`--pool-host`, `--pool-user`, ...).
- Connexion TCP au pool + envoi d'un frame `SetupConnection` SV2.
- Profils pools + paramètres perf + paramètres SV2.
- Logger JSON avec rotation.
- Compteurs de base exportables en format Prometheus text.
- Framing SV2 (encode/decode longueur + type + payload).
- État de session (connect/setup/channel/mining).
- SHA256 complet + SHA256d + sérialisation header 80 bytes.
- Boucle de scan nonce multi-thread (CPU demo).

> Note: le transport TLS/Noise SV2 complet et les messages submit/ack réseau restent à compléter pour un mineur production.

# frontend_flask/app/models.py

from dataclasses import dataclass

@dataclass
class LoadBalancingRule:
    path: str
    algorithm: str
    servers: list

@dataclass
class BackendServer:
    id: str
    ip: str
    port: int
    weight: int
# Containers and Kubernetes Basics (for system design)

Low-level container mechanics (namespaces, cgroups): `../../../OS/code/68_ContainersFromScratch`.
Deployment strategies: `23_DeploymentStrategies`; service discovery: `20_...`;
resilience: `19_...`; observability: `21_...`.

## Why orchestration
Containers give packaging and isolation; running many across many machines needs
scheduling, scaling, self-healing, networking, rollout, config/secrets, storage -
that is **Kubernetes (K8s)**: a declarative, reconciliation-based control plane.

## Architecture
- **Control plane:** `kube-apiserver` (the only entry point, REST + watch),
  **etcd** (Raft-replicated store of all cluster state - `22_ConsensusAndCoordination`),
  `kube-scheduler` (assigns Pods to nodes), `kube-controller-manager` (controllers),
  cloud-controller-manager.
- **Nodes:** `kubelet` (runs Pods via the container runtime - containerd/CRI-O -
  and reports status), `kube-proxy`/eBPF dataplane (Service load balancing), CNI plugin
  (Pod networking), CSI (storage).
- **Reconciliation loop:** you declare DESIRED state (YAML); controllers
  continuously compare to ACTUAL state and act (create Pods, reschedule on node
  failure). Level-triggered and idempotent: the core K8s design idea.

## Core objects
| Object | Purpose |
|---|---|
| **Pod** | smallest unit: 1+ containers sharing network namespace/volumes; ephemeral |
| **ReplicaSet / Deployment** | N identical stateless Pods; rolling updates and rollbacks |
| **StatefulSet** | stable identity + ordered rollout + per-Pod persistent volume (databases, Kafka) |
| **DaemonSet** | one Pod per node (log/metrics agents) |
| **Job / CronJob** | run-to-completion / scheduled |
| **Service** | stable virtual IP + DNS name load-balancing over Pods matched by labels (ClusterIP, NodePort, LoadBalancer) |
| **Ingress / Gateway API** | L7 HTTP routing, TLS termination into Services |
| **ConfigMap / Secret** | configuration / sensitive data (base64, not encrypted by default - enable encryption at rest, use external secret managers) |
| **PV / PVC / StorageClass** | persistent storage abstraction |
| **HPA / VPA / Cluster Autoscaler** | scale Pods by metrics / resize requests / add nodes |
| **Namespace, RBAC, NetworkPolicy, ResourceQuota** | multi-tenancy and security |

## Scheduling and resources
- Each container declares **requests** (scheduler reservation, used for placement and
  CPU shares) and **limits** (enforced by cgroups: CPU throttling, memory OOM kill).
  QoS classes: Guaranteed / Burstable / BestEffort (evicted first under pressure).
- Scheduler filters then scores nodes: resources, node/pod **affinity and
  anti-affinity**, **taints/tolerations**, topology spread (zones), priorities/
  preemption. PodDisruptionBudgets bound voluntary evictions.
- **Probes:** liveness (restart if dead), readiness (remove from Service endpoints
  until ready), startup (slow starters). Misconfigured liveness probes cause restart
  storms.
- Graceful shutdown: SIGTERM -> `preStop` hook -> drain connections within
  `terminationGracePeriodSeconds` -> SIGKILL; handle PID 1 and signal forwarding.

## Networking
Flat Pod network: every Pod gets an IP and can reach every other Pod without NAT
(implemented by CNI: Calico, Cilium (eBPF), Flannel). **Service** = stable
virtual IP load-balanced by iptables/IPVS/eBPF; **CoreDNS** gives
`svc.namespace.svc.cluster.local`; **Ingress** for north-south traffic;
**service mesh** (Istio, Linkerd) adds mTLS, retries, traffic splitting, telemetry
via sidecars or ambient mode - at a complexity/latency cost. NetworkPolicy =
firewall rules between Pods.

## Delivery patterns
Rolling update (default; `maxSurge`/`maxUnavailable`), blue/green (two Deployments,
switch Service selector), canary (traffic weights via Ingress/mesh/Argo Rollouts),
GitOps (Argo CD/Flux: Git is the source of truth, controllers apply it), Helm/
Kustomize for templating, operators/CRDs to encode operational knowledge
(databases, Kafka) as controllers.

## Design implications for services
- **Stateless Pods behind Services**, state in external stores/StatefulSets;
  12-factor config via env/ConfigMaps; idempotent startup; health endpoints.
- Expect Pods to be killed anytime (node drain, spot instances, autoscaling):
  graceful shutdown, retries with backoff (`19_...`), idempotency (`24_...`).
- Capacity: requests drive bin-packing and cost; overcommit risks throttling/OOM;
  autoscale on meaningful metrics (RPS, queue lag via KEDA), not only CPU.
- Observability: logs to stdout, metrics scraping (Prometheus), traces (OpenTelemetry).
- Multi-AZ spreading, multi-cluster/multi-region for blast-radius limits; etcd and
  control plane are critical (managed K8s hides them).
- Security: least-privilege RBAC and ServiceAccounts, image scanning/signing, Pod
  Security Standards (no privileged/hostPath), secrets management, network policies,
  admission controllers (OPA/Kyverno), runtime sandboxes for untrusted code.

## Senior interviewer Q&A
**Q: What happens when you `kubectl apply` a Deployment?**
A: The API server validates/stores it in etcd; the Deployment controller creates a
ReplicaSet; the ReplicaSet controller creates Pod objects; the scheduler binds each
Pod to a node; the kubelet there pulls the image, sets up namespaces/cgroups/CNI/
volumes via the runtime and starts containers, reporting status back; the Service/
EndpointSlice controller adds ready Pods to the Service.

**Q: How does K8s achieve self-healing?**
A: Controllers reconcile desired vs actual state: a crashed container is restarted
by the kubelet, a lost node's Pods are rescheduled by controllers after the node
is marked NotReady, failing readiness probes remove Pods from load balancing.

**Q: Requests vs limits - what are the consequences of getting them wrong?**
A: Too-low requests -> overpacked nodes and noisy neighbors; CPU limits too low ->
throttling latency spikes even with idle CPU; memory limit too low -> OOMKill;
no requests -> BestEffort, evicted first. Common guidance: set memory request =
limit, be careful with CPU limits.

**Q: How do you do zero-downtime deploys?**
A: Rolling update with readiness probes and `maxUnavailable: 0`, `preStop` sleep to
let endpoints propagate, graceful shutdown handling in-flight requests,
PodDisruptionBudgets, backward-compatible schema/API changes (expand-contract).

**Q: StatefulSet vs Deployment?**
A: StatefulSets give stable network identity (`pod-0`), ordered start/stop, and
per-replica PVCs that follow the Pod - needed for databases/brokers; Deployments
treat Pods as interchangeable.

**Q: When is Kubernetes the wrong choice?**
A: Small teams/simple apps (PaaS, serverless, a few VMs are cheaper to operate), hard
real-time or special hardware without operator support, or when the operational
complexity/cost outweighs the benefits.

**Q: Containers vs VMs vs serverless?**
A: VMs: strongest isolation, heavy, slow start. Containers: shared kernel, fast,
dense; orchestrator needed. Serverless/FaaS: scale-to-zero, no ops, cold starts,
execution limits, vendor coupling. Pick by isolation needs, utilization, startup
latency and operational capacity.

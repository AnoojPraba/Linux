# 35_ContainersAndKubernetesBasics

Kubernetes for system design: control plane and nodes, reconciliation loop, core objects, scheduling and resources, networking, delivery patterns and design implications, with a senior Q&A.

## Files
- `NOTES.md` - (120 lines) sections: Why orchestration; Architecture; Core objects; Scheduling and resources; Networking; Delivery patterns; Design implications for services; Senior interviewer Q&A

## How to use this note
- Drill: narrate what happens on `kubectl apply` of a Deployment, from API server to a Service endpoint.
- Rehearse the "Senior interviewer Q&A": apply flow, self-healing, requests vs limits, zero-downtime deploys, StatefulSet vs Deployment, when K8s is the wrong choice.
- Case-study answer framework: requirements -> estimates -> API -> data model -> architecture -> deep dives -> trade-offs (question bank: `../28_CommonInterviewQuestionsCheatSheet`).

## Key trade-offs / interview angles
- Declarative, level-triggered reconciliation: controllers continuously drive actual state toward desired state; etcd (Raft) stores cluster state.
- Objects: Pod, Deployment/ReplicaSet, StatefulSet (stable identity plus PVC), DaemonSet, Job, Service, Ingress/Gateway, ConfigMap/Secret (base64 only), PV/PVC, HPA/VPA/Cluster Autoscaler.
- Requests drive scheduling and cgroup shares; limits cause CPU throttling or OOM kill; QoS classes decide eviction order.
- Probes: liveness (restart), readiness (traffic), startup; bad liveness probes cause restart storms; graceful shutdown via SIGTERM and preStop.
- Networking: flat Pod network (CNI), Services (iptables/IPVS/eBPF), CoreDNS, Ingress, service mesh (mTLS, retries) at a latency/complexity cost.
- Design for killable Pods: stateless services, external state, retries and idempotency, autoscale on meaningful metrics, least-privilege RBAC and Pod security.

## Related
- `../23_DeploymentStrategies`
- `../20_ServiceDiscoveryAndAPIGateway`
- `../19_ResiliencePatterns`
- `../22_ConsensusAndCoordination`
- `../../../OS/code/68_ContainersFromScratch`

## Conventions when extending
- Keep the NOTES.md concise, bullet-style and interview-focused (no essay prose); new topic folders use the next two-digit prefix.
- Conceptual only: no code to build, so there are no binaries to commit.

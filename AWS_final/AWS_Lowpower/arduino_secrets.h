// GSM settings
#define SECRET_PINNUMBER     ""
//#define SECRET_GPRS_APN      "imis/internet" // replace your GPRS APN
#define SECRET_GPRS_APN      "airtelgprs.com"
#define SECRET_GPRS_LOGIN    ""    // replace with your GPRS login
#define SECRET_GPRS_PASSWORD "" // replace with your GPRS password

// Fill in the hostname of your AWS IoT broker
#define SECRET_BROKER "a3d3x4sh5k4yie-ats.iot.us-east-1.amazonaws.com"

#define MIN  0
#define MAX  10

// Fill in the boards public certificate
const char SECRET_CERTIFICATE[] = R"(
-----BEGIN CERTIFICATE-----
MIICgTCCAWmgAwIBAgIUFNXH/8Sl44fB8NTiIAK0bGZFzvMwDQYJKoZIhvcNAQEL
BQAwTTFLMEkGA1UECwxCQW1hem9uIFdlYiBTZXJ2aWNlcyBPPUFtYXpvbi5jb20g
SW5jLiBMPVNlYXR0bGUgU1Q9V2FzaGluZ3RvbiBDPVVTMB4XDTE5MDYyODEyNDgy
NFoXDTQ5MTIzMTIzNTk1OVowETEPMA0GA1UEAxMGdGVzdDI4MFkwEwYHKoZIzj0C
AQYIKoZIzj0DAQcDQgAEmwWfhoLLQg8LF0omD52RUAPonYbhrkgrB88zrqzTxrsJ
2CpM+69Y9hCfqk/1/g55u1dg+IfBIyJIvpt73v/MsqNgMF4wHwYDVR0jBBgwFoAU
Fa5HW1pridobGQHhw+nKwwtlFMEwHQYDVR0OBBYEFIjYh5CcVpnbfwQwqMwjYCSp
DDnnMAwGA1UdEwEB/wQCMAAwDgYDVR0PAQH/BAQDAgeAMA0GCSqGSIb3DQEBCwUA
A4IBAQBdrtyKmBHEPp+phzre9vkz3MV2nOev+sEroqAVbOtLx5VKjFRvP+THYuU4
d/vgBYzC3aMlLvu4NSFrMZg8gDcv2oLI0Z9svL2MEQcAxvHf9J5zxaObisgrzUvP
L1LQAHk12wA+V/Lng/76bm89ZHmuhPNi07/UoCoN3a/Q117UqR595+s4Wt7olMdY
nzkxm/tY+rrnf0p06oZPRYygTmH1gzjqMMaWs7arAE0Hhqqy61DOgkPjaHRZXjKY
KVlfGcGOJZoianAkgHPjnwfXvMkSsS537DGSgSLBoaZoESYo6iTxXzSFhigWaBVP
lmHpdbOtCDKUnUVYfqOVfXifpzev
-----END CERTIFICATE-----
)";

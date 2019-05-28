// GSM settings
#define SECRET_PINNUMBER     ""
#define SECRET_GPRS_APN      "imis/internet" // replace your GPRS APN
#define SECRET_GPRS_LOGIN    ""    // replace with your GPRS login
#define SECRET_GPRS_PASSWORD "" // replace with your GPRS password

// Fill in the hostname of your AWS IoT broker
#define SECRET_BROKER "a3d3x4sh5k4yie-ats.iot.ap-south-1.amazonaws.com"

// Fill in the boards public certificate
const char SECRET_CERTIFICATE[] = R"(
-----BEGIN CERTIFICATE-----
MIHMMHUCAQAwEzERMA8GA1UEAxMIbmV3dHJhaWwwWTATBgcqhkjOPQIBBggqhkjOPQMBBwNCAARC
Ayix7cA+CEDs9YOX2SDYf58cwLeGzrEBBOgAih/Rlz3rUYO3c92E+7xq1dFKuX1mPzQFybcdT31y
ZNV7AJuHoAAwCgYIKoZIzj0EAwIDRwAwRAIgNahxsQyv3v7vLuu1Hp83xsI3cIr2W5H9swZYVITd
1RwCIB8S4hOAhrIZlV4cOaqR9xDJGQYjvHtAf0/iZ35c37CU
-----END CERTIFICATE-----
)";

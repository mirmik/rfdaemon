#include <string>
#include <vector>
#include <utility>
#include <string.h>
#include <stdlib.h>

const char* const IRCC_RESOURCES_0 = 
		"\x74\x65\x78\x74\x73\x20\x3D\x20\x5B\x5D\x0A\x0A\x66\x75\x6E\x63\x74\x69"
		"\x6F\x6E\x20\x69\x6E\x69\x74\x5F\x66\x75\x6E\x63\x74\x69\x6F\x6E\x28\x29"
		"\x20\x0A\x7B\x0A\x20\x20\x20\x20\x76\x61\x72\x20\x78\x6D\x6C\x48\x74\x74"
		"\x70\x20\x3D\x20\x6E\x65\x77\x20\x58\x4D\x4C\x48\x74\x74\x70\x52\x65\x71"
		"\x75\x65\x73\x74\x28\x29\x3B\x0A\x20\x20\x20\x20\x78\x6D\x6C\x48\x74\x74"
		"\x70\x2E\x6F\x70\x65\x6E\x28\x20\x22\x47\x45\x54\x22\x2C\x20\x22\x61\x70"
		"\x70\x73\x5F\x73\x74\x61\x74\x65\x2E\x6A\x73\x6F\x6E\x22\x2C\x20\x66\x61"
		"\x6C\x73\x65\x20\x29\x3B\x20\x2F\x2F\x20\x66\x61\x6C\x73\x65\x20\x66\x6F"
		"\x72\x20\x73\x79\x6E\x63\x68\x72\x6F\x6E\x6F\x75\x73\x20\x72\x65\x71\x75"
		"\x65\x73\x74\x0A\x20\x20\x20\x20\x78\x6D\x6C\x48\x74\x74\x70\x2E\x73\x65"
		"\x6E\x64\x28\x20\x6E\x75\x6C\x6C\x20\x29\x3B\x0A\x20\x20\x20\x20\x72\x65"
		"\x73\x70\x6F\x6E\x73\x65\x20\x3D\x20\x78\x6D\x6C\x48\x74\x74\x70\x2E\x72"
		"\x65\x73\x70\x6F\x6E\x73\x65\x54\x65\x78\x74\x0A\x20\x20\x20\x20\x6A\x73"
		"\x6F\x6E\x20\x3D\x20\x4A\x53\x4F\x4E\x2E\x70\x61\x72\x73\x65\x28\x72\x65"
		"\x73\x70\x6F\x6E\x73\x65\x29\x3B\x0A\x20\x20\x20\x20\x76\x61\x72\x20\x64"
		"\x69\x76\x74\x65\x78\x74\x20\x3D\x20\x64\x6F\x63\x75\x6D\x65\x6E\x74\x2E"
		"\x67\x65\x74\x45\x6C\x65\x6D\x65\x6E\x74\x42\x79\x49\x64\x28\x22\x64\x69"
		"\x76\x74\x65\x78\x74\x22\x29\x3B\x0A\x20\x20\x20\x20\x64\x69\x76\x74\x65"
		"\x78\x74\x2E\x69\x6E\x6E\x65\x72\x48\x54\x4D\x4C\x20\x3D\x20\x22\x22\x3B"
		"\x0A\x0A\x20\x20\x20\x20\x66\x6F\x72\x20\x28\x76\x61\x72\x20\x69\x20\x3D"
		"\x20\x30\x3B\x20\x69\x20\x3C\x20\x6A\x73\x6F\x6E\x2E\x61\x70\x70\x73\x2E"
		"\x6C\x65\x6E\x67\x74\x68\x3B\x20\x69\x2B\x2B\x29\x20\x7B\x0A\x20\x20\x20"
		"\x20\x20\x20\x20\x20\x2F\x2F\x20\x63\x72\x65\x61\x74\x65\x20\x64\x69\x76"
		"\x0A\x20\x20\x20\x20\x20\x20\x20\x20\x6C\x65\x74\x20\x69\x6E\x64\x65\x78"
		"\x20\x3D\x20\x69\x0A\x20\x20\x20\x20\x20\x20\x20\x20\x76\x61\x72\x20\x64"
		"\x69\x76\x20\x3D\x20\x64\x6F\x63\x75\x6D\x65\x6E\x74\x2E\x63\x72\x65\x61"
		"\x74\x65\x45\x6C\x65\x6D\x65\x6E\x74\x28\x22\x64\x69\x76\x22\x29\x3B\x0A"
		"\x0A\x20\x20\x20\x20\x20\x20\x20\x20\x2F\x2F\x20\x63\x72\x65\x61\x74\x65"
		"\x20\x6C\x61\x62\x65\x6C\x20\x66\x6F\x72\x20\x74\x65\x78\x74\x0A\x20\x20"
		"\x20\x20\x20\x20\x20\x20\x76\x61\x72\x20\x74\x65\x78\x74\x20\x3D\x20\x64"
		"\x6F\x63\x75\x6D\x65\x6E\x74\x2E\x63\x72\x65\x61\x74\x65\x45\x6C\x65\x6D"
		"\x65\x6E\x74\x28\x22\x6C\x61\x62\x65\x6C\x22\x29\x3B\x0A\x20\x20\x20\x20"
		"\x20\x20\x20\x20\x74\x65\x78\x74\x73\x2E\x70\x75\x73\x68\x28\x74\x65\x78"
		"\x74\x29\x3B\x0A\x20\x20\x20\x20\x20\x20\x20\x20\x64\x69\x76\x2E\x61\x70"
		"\x70\x65\x6E\x64\x43\x68\x69\x6C\x64\x28\x74\x65\x78\x74\x29\x3B\x0A\x0A"
		"\x20\x20\x20\x20\x20\x20\x20\x20\x6C\x65\x74\x20\x73\x74\x6F\x70\x5F\x62"
		"\x74\x6E\x20\x3D\x20\x64\x6F\x63\x75\x6D\x65\x6E\x74\x2E\x63\x72\x65\x61"
		"\x74\x65\x45\x6C\x65\x6D\x65\x6E\x74\x28\x22\x62\x75\x74\x74\x6F\x6E\x22"
		"\x29\x3B\x0A\x20\x20\x20\x20\x20\x20\x20\x20\x73\x74\x6F\x70\x5F\x62\x74"
		"\x6E\x2E\x69\x6E\x6E\x65\x72\x48\x54\x4D\x4C\x20\x3D\x20\x22\x53\x74\x6F"
		"\x70\x22\x3B\x0A\x20\x20\x20\x20\x20\x20\x20\x20\x73\x74\x6F\x70\x5F\x62"
		"\x74\x6E\x2E\x6F\x6E\x63\x6C\x69\x63\x6B\x20\x3D\x20\x66\x75\x6E\x63\x74"
		"\x69\x6F\x6E\x20\x28\x29\x20\x7B\x0A\x20\x20\x20\x20\x20\x20\x20\x20\x20"
		"\x20\x20\x20\x73\x74\x6F\x70\x28\x69\x6E\x64\x65\x78\x29\x3B\x0A\x20\x20"
		"\x20\x20\x20\x20\x20\x20\x7D\x3B\x0A\x20\x20\x20\x20\x20\x20\x20\x20\x64"
		"\x69\x76\x2E\x61\x70\x70\x65\x6E\x64\x43\x68\x69\x6C\x64\x28\x73\x74\x6F"
		"\x70\x5F\x62\x74\x6E\x29\x3B\x0A\x0A\x20\x20\x20\x20\x20\x20\x20\x20\x6C"
		"\x65\x74\x20\x73\x74\x61\x72\x74\x5F\x62\x74\x6E\x20\x3D\x20\x64\x6F\x63"
		"\x75\x6D\x65\x6E\x74\x2E\x63\x72\x65\x61\x74\x65\x45\x6C\x65\x6D\x65\x6E"
		"\x74\x28\x22\x62\x75\x74\x74\x6F\x6E\x22\x29\x3B\x0A\x20\x20\x20\x20\x20"
		"\x20\x20\x20\x73\x74\x61\x72\x74\x5F\x62\x74\x6E\x2E\x69\x6E\x6E\x65\x72"
		"\x48\x54\x4D\x4C\x20\x3D\x20\x22\x53\x74\x61\x72\x74\x22\x3B\x0A\x20\x20"
		"\x20\x20\x20\x20\x20\x20\x73\x74\x61\x72\x74\x5F\x62\x74\x6E\x2E\x6F\x6E"
		"\x63\x6C\x69\x63\x6B\x20\x3D\x20\x66\x75\x6E\x63\x74\x69\x6F\x6E\x20\x28"
		"\x29\x20\x7B\x0A\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x73\x74"
		"\x61\x72\x74\x28\x69\x6E\x64\x65\x78\x29\x3B\x0A\x20\x20\x20\x20\x20\x20"
		"\x20\x20\x7D\x3B\x0A\x20\x20\x20\x20\x20\x20\x20\x20\x64\x69\x76\x2E\x61"
		"\x70\x70\x65\x6E\x64\x43\x68\x69\x6C\x64\x28\x73\x74\x61\x72\x74\x5F\x62"
		"\x74\x6E\x29\x3B\x0A\x0A\x20\x20\x20\x20\x20\x20\x20\x20\x64\x69\x76\x74"
		"\x65\x78\x74\x2E\x61\x70\x70\x65\x6E\x64\x43\x68\x69\x6C\x64\x28\x64\x69"
		"\x76\x29\x3B\x0A\x20\x20\x20\x20\x7D\x0A\x7D\x0A\x0A\x66\x75\x6E\x63\x74"
		"\x69\x6F\x6E\x20\x73\x74\x61\x74\x65\x5F\x75\x70\x64\x61\x74\x65\x5F\x6C"
		"\x6F\x6F\x70\x28\x29\x20\x0A\x7B\x0A\x20\x20\x20\x20\x73\x65\x74\x49\x6E"
		"\x74\x65\x72\x76\x61\x6C\x28\x66\x75\x6E\x63\x74\x69\x6F\x6E\x28\x29\x7B"
		"\x0A\x20\x20\x20\x20\x20\x20\x20\x20\x76\x61\x72\x20\x78\x6D\x6C\x48\x74"
		"\x74\x70\x20\x3D\x20\x6E\x65\x77\x20\x58\x4D\x4C\x48\x74\x74\x70\x52\x65"
		"\x71\x75\x65\x73\x74\x28\x29\x3B\x0A\x20\x20\x20\x20\x20\x20\x20\x20\x78"
		"\x6D\x6C\x48\x74\x74\x70\x2E\x6F\x70\x65\x6E\x28\x20\x22\x47\x45\x54\x22"
		"\x2C\x20\x22\x61\x70\x70\x73\x5F\x73\x74\x61\x74\x65\x2E\x6A\x73\x6F\x6E"
		"\x22\x2C\x20\x74\x72\x75\x65\x20\x29\x3B\x20\x2F\x2F\x20\x66\x61\x6C\x73"
		"\x65\x20\x66\x6F\x72\x20\x73\x79\x6E\x63\x68\x72\x6F\x6E\x6F\x75\x73\x20"
		"\x72\x65\x71\x75\x65\x73\x74\x0A\x20\x20\x20\x20\x20\x20\x20\x20\x78\x6D"
		"\x6C\x48\x74\x74\x70\x2E\x6F\x6E\x6C\x6F\x61\x64\x20\x3D\x20\x66\x75\x6E"
		"\x63\x74\x69\x6F\x6E\x20\x28\x65\x29\x20\x7B\x0A\x20\x20\x20\x20\x20\x20"
		"\x20\x20\x20\x20\x20\x20\x72\x65\x73\x70\x6F\x6E\x73\x65\x20\x3D\x20\x78"
		"\x6D\x6C\x48\x74\x74\x70\x2E\x72\x65\x73\x70\x6F\x6E\x73\x65\x54\x65\x78"
		"\x74\x0A\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x6A\x73\x6F\x6E"
		"\x20\x3D\x20\x4A\x53\x4F\x4E\x2E\x70\x61\x72\x73\x65\x28\x72\x65\x73\x70"
		"\x6F\x6E\x73\x65\x29\x3B\x0A\x0A\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20"
		"\x20\x20\x66\x6F\x72\x20\x28\x76\x61\x72\x20\x69\x20\x3D\x20\x30\x3B\x20"
		"\x69\x20\x3C\x20\x6A\x73\x6F\x6E\x2E\x61\x70\x70\x73\x2E\x6C\x65\x6E\x67"
		"\x74\x68\x3B\x20\x69\x2B\x2B\x29\x20\x7B\x0A\x20\x20\x20\x20\x20\x20\x20"
		"\x20\x20\x20\x20\x20\x20\x20\x20\x20\x74\x65\x78\x74\x20\x3D\x20\x74\x65"
		"\x78\x74\x73\x5B\x69\x5D\x3B\x0A\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20"
		"\x20\x20\x20\x20\x20\x20\x74\x65\x78\x74\x2E\x69\x6E\x6E\x65\x72\x48\x54"
		"\x4D\x4C\x20\x3D\x20\x6A\x73\x6F\x6E\x2E\x61\x70\x70\x73\x5B\x69\x5D\x2E"
		"\x6E\x61\x6D\x65\x20\x2B\x20\x22\x3A\x20\x22\x20\x2B\x20\x6A\x73\x6F\x6E"
		"\x2E\x61\x70\x70\x73\x5B\x69\x5D\x2E\x73\x74\x61\x74\x65\x3B\x0A\x20\x20"
		"\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x7D\x0A\x20\x20\x20\x20\x20\x20"
		"\x20\x20\x7D\x3B\x0A\x20\x20\x20\x20\x20\x20\x20\x20\x78\x6D\x6C\x48\x74"
		"\x74\x70\x2E\x73\x65\x6E\x64\x28\x20\x6E\x75\x6C\x6C\x20\x29\x3B\x0A\x20"
		"\x20\x20\x20\x7D\x2C\x20\x31\x30\x30\x29\x3B\x0A\x7D\x0A\x0A\x66\x75\x6E"
		"\x63\x74\x69\x6F\x6E\x20\x73\x74\x6F\x70\x5F\x61\x6C\x6C\x28\x29\x20\x0A"
		"\x7B\x0A\x20\x20\x20\x20\x76\x61\x72\x20\x78\x6D\x6C\x48\x74\x74\x70\x20"
		"\x3D\x20\x6E\x65\x77\x20\x58\x4D\x4C\x48\x74\x74\x70\x52\x65\x71\x75\x65"
		"\x73\x74\x28\x29\x3B\x0A\x20\x20\x20\x20\x78\x6D\x6C\x48\x74\x74\x70\x2E"
		"\x6F\x70\x65\x6E\x28\x20\x22\x47\x45\x54\x22\x2C\x20\x22\x73\x74\x6F\x70"
		"\x5F\x61\x6C\x6C\x2E\x61\x63\x74\x69\x6F\x6E\x22\x2C\x20\x66\x61\x6C\x73"
		"\x65\x20\x29\x3B\x20\x2F\x2F\x20\x66\x61\x6C\x73\x65\x20\x66\x6F\x72\x20"
		"\x73\x79\x6E\x63\x68\x72\x6F\x6E\x6F\x75\x73\x20\x72\x65\x71\x75\x65\x73"
		"\x74\x0A\x20\x20\x20\x20\x78\x6D\x6C\x48\x74\x74\x70\x2E\x73\x65\x6E\x64"
		"\x28\x20\x6E\x75\x6C\x6C\x20\x29\x3B\x0A\x7D\x0A\x0A\x66\x75\x6E\x63\x74"
		"\x69\x6F\x6E\x20\x73\x74\x61\x72\x74\x5F\x61\x6C\x6C\x28\x29\x20\x0A\x7B"
		"\x0A\x20\x20\x20\x20\x76\x61\x72\x20\x78\x6D\x6C\x48\x74\x74\x70\x20\x3D"
		"\x20\x6E\x65\x77\x20\x58\x4D\x4C\x48\x74\x74\x70\x52\x65\x71\x75\x65\x73"
		"\x74\x28\x29\x3B\x0A\x20\x20\x20\x20\x78\x6D\x6C\x48\x74\x74\x70\x2E\x6F"
		"\x70\x65\x6E\x28\x20\x22\x47\x45\x54\x22\x2C\x20\x22\x73\x74\x61\x72\x74"
		"\x5F\x61\x6C\x6C\x2E\x61\x63\x74\x69\x6F\x6E\x22\x2C\x20\x66\x61\x6C\x73"
		"\x65\x20\x29\x3B\x20\x2F\x2F\x20\x66\x61\x6C\x73\x65\x20\x66\x6F\x72\x20"
		"\x73\x79\x6E\x63\x68\x72\x6F\x6E\x6F\x75\x73\x20\x72\x65\x71\x75\x65\x73"
		"\x74\x0A\x20\x20\x20\x20\x78\x6D\x6C\x48\x74\x74\x70\x2E\x73\x65\x6E\x64"
		"\x28\x20\x6E\x75\x6C\x6C\x20\x29\x3B\x0A\x7D\x0A\x0A\x66\x75\x6E\x63\x74"
		"\x69\x6F\x6E\x20\x73\x74\x6F\x70\x28\x69\x29\x20\x0A\x7B\x0A\x20\x20\x20"
		"\x20\x76\x61\x72\x20\x78\x6D\x6C\x48\x74\x74\x70\x20\x3D\x20\x6E\x65\x77"
		"\x20\x58\x4D\x4C\x48\x74\x74\x70\x52\x65\x71\x75\x65\x73\x74\x28\x29\x3B"
		"\x0A\x20\x20\x20\x20\x78\x6D\x6C\x48\x74\x74\x70\x2E\x6F\x70\x65\x6E\x28"
		"\x20\x22\x47\x45\x54\x22\x2C\x20\x22\x73\x74\x6F\x70\x2E\x61\x63\x74\x69"
		"\x6F\x6E\x3F\x69\x6E\x64\x65\x78\x3D\x22\x20\x2B\x20\x69\x2C\x20\x66\x61"
		"\x6C\x73\x65\x20\x29\x3B\x20\x2F\x2F\x20\x66\x61\x6C\x73\x65\x20\x66\x6F"
		"\x72\x20\x73\x79\x6E\x63\x68\x72\x6F\x6E\x6F\x75\x73\x20\x72\x65\x71\x75"
		"\x65\x73\x74\x0A\x20\x20\x20\x20\x78\x6D\x6C\x48\x74\x74\x70\x2E\x73\x65"
		"\x6E\x64\x28\x20\x6E\x75\x6C\x6C\x20\x29\x3B\x0A\x7D\x20\x0A\x0A\x66\x75"
		"\x6E\x63\x74\x69\x6F\x6E\x20\x73\x74\x61\x72\x74\x28\x69\x29\x20\x0A\x7B"
		"\x0A\x20\x20\x20\x20\x76\x61\x72\x20\x78\x6D\x6C\x48\x74\x74\x70\x20\x3D"
		"\x20\x6E\x65\x77\x20\x58\x4D\x4C\x48\x74\x74\x70\x52\x65\x71\x75\x65\x73"
		"\x74\x28\x29\x3B\x0A\x20\x20\x20\x20\x78\x6D\x6C\x48\x74\x74\x70\x2E\x6F"
		"\x70\x65\x6E\x28\x20\x22\x47\x45\x54\x22\x2C\x20\x22\x73\x74\x61\x72\x74"
		"\x2E\x61\x63\x74\x69\x6F\x6E\x3F\x69\x6E\x64\x65\x78\x3D\x22\x20\x2B\x20"
		"\x69\x2C\x20\x66\x61\x6C\x73\x65\x20\x29\x3B\x20\x2F\x2F\x20\x66\x61\x6C"
		"\x73\x65\x20\x66\x6F\x72\x20\x73\x79\x6E\x63\x68\x72\x6F\x6E\x6F\x75\x73"
		"\x20\x72\x65\x71\x75\x65\x73\x74\x0A\x20\x20\x20\x20\x78\x6D\x6C\x48\x74"
		"\x74\x70\x2E\x73\x65\x6E\x64\x28\x20\x6E\x75\x6C\x6C\x20\x29\x3B\x0A\x7D"
		"\x20";

const char* const IRCC_RESOURCES_1 = 
		"\x3C\x21\x44\x4F\x43\x54\x59\x50\x45\x20\x48\x54\x4D\x4C\x3E\x0A\x3C\x68"
		"\x74\x6D\x6C\x3E\x0A\x20\x3C\x68\x65\x61\x64\x3E\x0A\x20\x20\x3C\x73\x63"
		"\x72\x69\x70\x74\x20\x74\x79\x70\x65\x3D\x22\x74\x65\x78\x74\x2F\x6A\x61"
		"\x76\x61\x73\x63\x72\x69\x70\x74\x22\x20\x73\x72\x63\x3D\x22\x66\x75\x6E"
		"\x63\x74\x69\x6F\x6E\x73\x2E\x6A\x73\x22\x3E\x20\x3C\x2F\x73\x63\x72\x69"
		"\x70\x74\x3E\x0A\x20\x20\x3C\x6D\x65\x74\x61\x20\x63\x68\x61\x72\x73\x65"
		"\x74\x3D\x22\x75\x74\x66\x2D\x38\x22\x3E\x0A\x20\x20\x3C\x74\x69\x74\x6C"
		"\x65\x3E\xD0\xA2\xD0\xB5\xD0\xB3\x20\x42\x55\x54\x54\x4F\x4E\x3C\x2F\x74"
		"\x69\x74\x6C\x65\x3E\x0A\x20\x3C\x2F\x68\x65\x61\x64\x3E\x0A\x20\x20\x20"
		"\x20\x3C\x62\x6F\x64\x79\x3E\x0A\x20\x20\x20\x20\x20\x20\x20\x20\x3C\x64"
		"\x69\x76\x20\x69\x64\x3D\x22\x64\x69\x76\x74\x65\x78\x74\x22\x3E\x54\x45"
		"\x58\x54\x3C\x2F\x64\x69\x76\x3E\x0A\x0A\x20\x20\x20\x20\x20\x20\x20\x20"
		"\x3C\x73\x63\x72\x69\x70\x74\x20\x74\x79\x70\x65\x3D\x22\x74\x65\x78\x74"
		"\x2F\x6A\x61\x76\x61\x73\x63\x72\x69\x70\x74\x22\x3E\x0A\x20\x20\x20\x20"
		"\x20\x20\x20\x20\x20\x20\x20\x20\x69\x6E\x69\x74\x5F\x66\x75\x6E\x63\x74"
		"\x69\x6F\x6E\x28\x29\x0A\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20"
		"\x73\x74\x61\x74\x65\x5F\x75\x70\x64\x61\x74\x65\x5F\x6C\x6F\x6F\x70\x28"
		"\x29\x3B\x0A\x20\x20\x20\x20\x20\x20\x20\x20\x3C\x2F\x73\x63\x72\x69\x70"
		"\x74\x3E\x0A\x0A\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x3C\x62"
		"\x75\x74\x74\x6F\x6E\x20\x6F\x6E\x63\x6C\x69\x63\x6B\x3D\x22\x73\x74\x6F"
		"\x70\x5F\x61\x6C\x6C\x28\x29\x22\x3E\x73\x74\x6F\x70\x20\x61\x6C\x6C\x3C"
		"\x2F\x62\x75\x74\x74\x6F\x6E\x3E\x0A\x20\x20\x20\x20\x20\x20\x20\x20\x20"
		"\x20\x20\x20\x3C\x62\x75\x74\x74\x6F\x6E\x20\x6F\x6E\x63\x6C\x69\x63\x6B"
		"\x3D\x22\x73\x74\x61\x72\x74\x5F\x61\x6C\x6C\x28\x29\x22\x3E\x73\x74\x61"
		"\x72\x74\x20\x61\x6C\x6C\x3C\x2F\x62\x75\x74\x74\x6F\x6E\x3E\x0A\x20\x20"
		"\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x3C\x62\x75\x74\x74\x6F\x6E\x20"
		"\x6F\x6E\x63\x6C\x69\x63\x6B\x3D\x22\x73\x74\x6F\x70\x28\x31\x33\x29\x22"
		"\x3E\x73\x74\x6F\x70\x20\x31\x33\x3C\x2F\x62\x75\x74\x74\x6F\x6E\x3E\x0A"
		"\x20\x20\x20\x20\x3C\x2F\x62\x6F\x64\x79\x3E\x0A\x3C\x2F\x68\x74\x6D\x6C"
		"\x3E\x0A";

struct key_value_size
{
    const char *key;
    const char *value;
    size_t size;
};

struct key_value_size IRCC_RESOURCES_[] = {
	{"/web/functions.js", IRCC_RESOURCES_0, 2377},
	{"/web/index.html", IRCC_RESOURCES_1, 524},
	{NULL, NULL, 0}};

struct key_value_size *ircc_binary_search(const char *key)
{
    int low = 0;
    int high = sizeof(IRCC_RESOURCES_) / sizeof(IRCC_RESOURCES_[0]) - 1;
    int mid;
    while (low <= high)
    {
        mid = (low + high) / 2;
        int cmp = strcmp(key, IRCC_RESOURCES_[mid].key);
        if (cmp < 0)
            high = mid - 1;
        else if (cmp > 0)
            low = mid + 1;
        else
            return &IRCC_RESOURCES_[mid];
    }
    return NULL;
}

#ifdef __cplusplus
extern "C" const char *ircc_c_string(const char *key, size_t *sizeptr);
#endif
const char *ircc_c_string(const char *key, size_t *sizeptr)
{
    struct key_value_size *kvs = ircc_binary_search(key);
    if (kvs == NULL)
        return NULL;
    if (sizeptr != NULL)
        *sizeptr = kvs->size;
    return kvs->value;
}

#ifdef __cplusplus
extern "C" const char *ircc_name_by_no(size_t no);
#endif
const char *ircc_name_by_no(size_t no)
{
    return IRCC_RESOURCES_[no].key;
}

std::string ircc_string(const std::string& key)
{
    struct key_value_size *kvs = ircc_binary_search(key.c_str());
    if (kvs == NULL)
        return {};
    return std::string(kvs->value, kvs->size);
}

std::vector<uint8_t> ircc_vector(const std::string& key)
{
    struct key_value_size *kvs = ircc_binary_search(key.c_str());
    if (kvs == NULL)
        return {};
    return std::vector<uint8_t>((const uint8_t*)kvs->value, 
                (const uint8_t*)(kvs->value + kvs->size));
}

std::pair<const char*, size_t> ircc_pair(const std::string& key)
{
    struct key_value_size *kvs = ircc_binary_search(key.c_str());
    if (kvs == NULL)
        return {};
    return std::pair<const char*, size_t>(kvs->value, kvs->size);
}

std::vector<std::string> ircc_keys()
{
    std::vector<std::string> list;
    for (int i = 0; IRCC_RESOURCES_[i].key != NULL; i++)
    {
        list.push_back(IRCC_RESOURCES_[i].key);
    }
    return list;
}

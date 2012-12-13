#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/dkio.h>
#include <sys/vtoc.h>
#include <unistd.h>
#include <stropts.h>
#include <string.h>


static void print_geom(struct dk_geom *g)
{
	printf("geom = {\n");
	printf("\tdkg_ncyl = %u\n", g->dkg_ncyl);
	printf("\tdkg_acyl = %u\n", g->dkg_acyl);
	printf("\tdkg_bcyl = %u\n", g->dkg_bcyl);
	printf("\tdkg_nhead = %u\n", g->dkg_nhead);
	printf("\tdkg_nsect = %u\n", g->dkg_nsect);
	printf("\tdkg_intrlv = %u\n", g->dkg_intrlv);
	printf("\tdkg_apc = %u\n", g->dkg_apc);
	printf("\tdkg_rpm = %u\n", g->dkg_rpm);
	printf("\tdkg_pcyl = %u\n", g->dkg_pcyl);
	printf("\tdkg_write_reinstruct = %u\n", g->dkg_write_reinstruct);
	printf("\tdkg_read_reinstruct = %u\n", g->dkg_read_reinstruct);
	printf("}\n");
}

static void print_cinfo(struct dk_cinfo *i)
{
	printf("cinfo = {\n");
	printf("\tdki_cname[] = '%s'\n", i->dki_cname);
	printf("\tdki_ctype = %u\n", i->dki_ctype);
	printf("\tdki_flags = %u (0x%x)\n", i->dki_flags, i->dki_flags);
	printf("\tdki_cnum = %u\n", i->dki_cnum);
	printf("\tdki_addr = %u (0x%x)\n", i->dki_addr, i->dki_addr);
	printf("\tdki_space = %u (0x%x)\n", i->dki_space, i->dki_space);
	printf("\tdki_prio = %u\n", i->dki_prio);
	printf("\tdki_vec = %u\n", i->dki_vec);
	printf("\tdki_dname[] = '%s'\n", i->dki_dname);
	printf("\tdki_unit = %u\n", i->dki_unit);
	printf("\tdki_slave = %u\n", i->dki_slave);
	printf("\tdki_partition = %u\n", i->dki_partition);
	printf("\tdki_maxtransfer = %u\n", i->dki_maxtransfer);
	printf("}\n");
}

static void print_minfo(struct dk_minfo *m)
{
	printf("minfo = {\n");
	printf("\tdki_media_type = %u (0x%x)\n", m->dki_media_type,
		m->dki_media_type);
	printf("\tdki_lbsize = %u\n", m->dki_lbsize);
	printf("\tdki_capacity = %llu\n", m->dki_capacity);
	printf("}\n");
}

static void print_exvt(struct extvtoc *e)
{
	int i;

	printf("exvtoc = {\n");
	printf("\tv_bootinfo = { %lu, %lu, %lu }\n",
		e->v_bootinfo[0], e->v_bootinfo[1], e->v_bootinfo[2]);
	printf("\tv_sanity = %lx\n", e->v_sanity);
	printf("\tv_version = %lu\n", e->v_version);
	printf("\tv_volume = '%s'\n", e->v_volume);
	printf("\tv_sectorsz = %u\n", e->v_sectorsz);
	printf("\tv_nparts = %u\n", e->v_nparts);
	printf("\tv_part = {\n");

	for (i = 0; i < e->v_nparts; i++) {
		printf("\t\t{\n");
		printf("\t\t\tp_tag = %u\n", e->v_part[i].p_tag);
		printf("\t\t\tp_flag = 0x%x\n", e->v_part[i].p_flag);
		printf("\t\t\tp_start = %llu\n", e->v_part[i].p_start);
		printf("\t\t\tp_size = %llu\n", e->v_part[i].p_size);
		printf("\t\t},\n");
	}

	printf("\t}\n");
	printf("\tv_asciilabel = '%s'\n", e->v_asciilabel);
}

int main(int argc, char *argv[])
{
	int fd;

	struct dk_cinfo cinfo;
	struct extvtoc exvt;
	struct dk_minfo minfo;
	struct dk_geom geom;
	int removable;

	int ret;

	removable = 0;
	memset(&cinfo, 0, sizeof(cinfo));
	memset(&exvt, 0, sizeof(exvt));
	memset(&minfo, 0, sizeof(minfo));
	memset(&geom, 0, sizeof(geom));

	if (argc != 2) {
		fprintf(stderr,
			"\tusage: %s <devnode>\n", argv[0]);
		return 1;
	}

	printf("Opening...\n");
	fd = open(argv[1], O_RDWR | O_NDELAY);
	if (fd < 0) {
		perror("Can't open device");
		return 1;
	}

	/* That's the order format issues the ioctls, let's follow */

	printf("Issuing DKIOCINFO\n");
	ret = ioctl(fd, DKIOCINFO, &cinfo);
	if (ret == 0) {
		print_cinfo(&cinfo);
		printf("\n");
	} else {
		perror("DKIOCINFO failed");
	}

	printf("Issuing DKIOCREMOVABLE\n");
	ret = ioctl(fd, DKIOCREMOVABLE, &removable);
	if (ret == 0) {
		printf("Removable: %d\n", removable);
		printf("\n");
	} else {
		perror("DKIOCREMOVABLE failed");
	}


	printf("Issuing DKIOCGMEDIAINFO\n");
	ret = ioctl(fd, DKIOCGMEDIAINFO, &minfo);
	if (ret == 0) {
		print_minfo(&minfo);
		printf("\n");
	} else {
		perror("DKIOCGMEDIAINFO failed");
	}

	printf("Issuing DKIOCGEXTVTOC\n");
	ret = ioctl(fd, DKIOCGEXTVTOC, &exvt);
	if (ret == 0) {
		print_exvt(&exvt);
		printf("\n");
	} else {
		perror("DKIOCGEXTVTOC failed");
	}

	printf("Issuing DKIOCGGEOM\n");
	ret = ioctl(fd, DKIOCGGEOM, &geom);
	if (ret == 0) {
		print_geom(&geom);
		printf("\n");
	} else {
		perror("DKIOCGGEOM failed");
	}
	printf("Done.\n");

	return 0;
}


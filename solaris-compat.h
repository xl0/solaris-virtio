
#include <sys/sunddi.h>
#include <sys/cmn_err.h>
#include <sys/log.h>

#if !defined(offsetof)
#define offsetof(s, m)  ((size_t)(&(((s *)0)->m)))
#endif /* !offsetof */

#define container_of(m, s, name)                        \
	(void *)((uintptr_t)(m) - (uintptr_t)offsetof(s, name))

#define	TRACE { \
	cmn_err(CE_NOTE, "%s:%d %s()\n", __FILE__, __LINE__, __func__); \
}

#if (SOLARIS_COMPAT_VERSION == 11)
#define __builtin_stdarg_start __builtin_va_start 
#endif

/*PRINTFLIKE3*/
static void
dev_err(dev_info_t *dip, int ce, char *fmt, ...)
{
	va_list ap;
	char buf[LOG_MSGSIZE];

	(void) snprintf(buf, sizeof (buf), "%s%d: %s",
	    ddi_driver_name(dip), ddi_get_instance(dip), fmt);

	va_start(ap, fmt);
	vcmn_err(ce, buf, ap);
	va_end(ap);
}


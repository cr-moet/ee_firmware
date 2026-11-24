#ifndef _HAL_CLK_H
#define _HAL_CLK_H

#include <hal/types.h>

struct clk;

struct clk_ops {
	int		        (*enable)(struct clk *clk);
	void		    (*disable)(struct clk *clk);
	unsigned long	(*get_rate)(struct clk *clk);	/* Optional */
};

struct clk {
	const char		        *name;
	const struct clk_ops	*ops;		/* Can be NULL */
	struct clk		        *parent;	/* NULL for a root clock */
	unsigned int		    enable_count;
	struct clk		        *next;
};

/* Provider API. Clock drivers use this function. */
int clk_register(struct clk *clk);

/* Consumer API. Drivers and boards use these functions. */
struct clk *clk_get(const char *name);
int clk_enable(struct clk *clk);
void clk_disable(struct clk *clk);
unsigned long clk_get_rate(struct clk *clk);

#endif /* _HAL_CLK_H */
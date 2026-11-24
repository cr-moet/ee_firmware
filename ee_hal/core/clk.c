#include <string.h>
#include <hal/types.h>
#include <hal/errno.h>
#include <hal/clk.h>

static struct clk *clk_list;

int clk_register(struct clk *clk)
{
	if (!clk || !clk->name)
		return -EINVAL;
	if (clk_get(clk->name))
		return -EBUSY;

	clk->enable_count = 0;
	clk->next = clk_list;
	clk_list = clk;
	return 0;
}

struct clk *clk_get(const char *name)
{
	struct clk *clk;

	for (clk = clk_list; clk; clk = clk->next) {
		if (!strcmp(clk->name, name))
			return clk;
	}
	return NULL;
}

int clk_enable(struct clk *clk)
{
	int ret;

	if (!clk)
		return -EINVAL;

	/* The clock is already on. Only increment the counter. */
	if (clk->enable_count++)
		return 0;

	/* Enable the parent clock first. */
	ret = clk_enable(clk->parent);
	if (ret && clk->parent)
		goto err;

	if (clk->ops && clk->ops->enable) {
		ret = clk->ops->enable(clk);
		if (ret) {
			clk_disable(clk->parent);
			goto err;
		}
	}
	return 0;

err:
	clk->enable_count--;
	return ret;
}

void clk_disable(struct clk *clk)
{
	if (!clk || !clk->enable_count)
		return;

	/* Other users still need the clock. */
	if (--clk->enable_count)
		return;

	if (clk->ops && clk->ops->disable)
		clk->ops->disable(clk);

	clk_disable(clk->parent);
}

unsigned long clk_get_rate(struct clk *clk)
{
	if (!clk)
		return 0;
	if (clk->ops && clk->ops->get_rate)
		return clk->ops->get_rate(clk);

	/* A clock without get_rate has the rate of its parent. */
	return clk_get_rate(clk->parent);
}
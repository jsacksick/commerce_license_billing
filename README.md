Commerce License Billing
========================

Commerce License Billing provides advanced (prepaid, postpaid, prorated, plan-based, metered)
recurring billing for licenses.

Dependencies: Commerce License, Bundleswitcher, Commerce Card on File.

Getting started
---------------
1) Go to admin/config/licenses/billing-cycle-types and add a billing cycle type.
2) Create a product, select a license type, then below select your billing cycle type
and payment type (prepaid or postpaid).
3) Checkout the product. If you selected postpaid as the payment type, your product
will be free.
4) A billing cycle has now been opened (with the current start date, and the end date
depending on your billing cycle type settings), along with a matching recurring order.
5) When the billing cycle expires, the recurring order will be closed and charged for
using Commerce Card on File, and a new billing cycle & order will be opened.

Prepaid billing
---------------
Prepaid products are paid up front.
That means that if a customer registers on April 1st, he will immediately pay the
monthly fee for April. On the first day of May, he will be charged for the
april usage (if any), and the monthly fee for May. If on May 15h he cancels
his subscription, on the first day of June he will only pay the prorated
usage for May.
The other half of the May monthly fee will not be refunded, since that is
not currently implemented (a common strategy being to award the customer
points to be used for discounting future purchases).

Postpaid billing
----------------
Postpaid products are paid at the end of the billing cycle.
That means that if the customer registers on April 1st, his order is free and he pays
nothing. On the first day of May, he will be charged for the April monthly fee,
and the april usage (if any). If on May 15th he cancels his subscription, on the
first day of June he will pay the prorated montly fee for May, and the prorated usage
for May.

Prorated payments
-----------------
A prorated payment is a payment proportional to the duration of the usage.
So, if the billing cycle is two weeks, but the plan was used for one week,
only half of the plan's price will be set on the line item.

The plan and usage records have start and end timestamps.
If the end timestamp is missing, it is assumed that the record is still active / in progress,
so the current timestamp is taken as the end instead in order to give a cost estimation.
The duration (end - start) is compared to the billing cycle duration, and the record is priced proportionally.

Plan-based billing
------------------
Each license has one plan at a given point of time, which is the referenced product.
Different plans are represented by different products, all pointing to the
same license type / billing cycle type / payment type.
The module tracks plan changes in the cl_product_history table, that has a
start and an end date for each license and product_id.
So if the user switches $license->product_id from id: 10 ("Small plan") to
id: 11 ("Large plan"), the cl_product_history table will look like this:
license_id | product_id | start      | end
1          | 10         | 1364772240 | 1365722640
1          | 11         | 1367272800 | 0
The first row was created automatically when the billing cycle was opened,
and the start is the start of the billing cycle.
The second row has no end value because the plan is still active.
The start and end times are used for prorating plans during later billing
(allowing you to charge the customer for both plans, proportionaly to the
time they were used).

Metered (usage-based) billing
-----------------------------
If a license type implements the CommerceLicenseBillingUsageInterface interface
and declares its usage groups, the module will allow usage to be
registered and calculated for each usage group separately, and charge for it
at the end of the billing cycle.

Usage is reported asynchronously, and it is your job to call
commerce_license_billing_usage_add() and register usage (after an API call
received through Services, or after contacting the service yourself on cron, etc).
If a license has usage groups, the billing cycle won't be closed until
all usage has been reported (start - end pairs cover the entire billing cycle
duration).

There are two types of usage groups: counter and gauge.
- The counter tracks usage over time, and is always charged for in total.
For example if the following bandwidth usage was reported:
"Jan 1st - Jan 15th; 1024" and "Jan 15th - Jan 31st; 128", there
will be one line item, charging for 1052mb of usage.
- The gauge tracks discrete usage values over time, and each
value is charged for separately. For example, if the following env
usage is reported "Jan 1st - Jan 15th; 2" and "Jan 15th - Jan 31st; 4",
there will be two prorated line items, charging for 2 and 4 environments.
The gauge type also allows for open-ended usage (immediate => TRUE), in which
case it is carried over into the next billing cycles.

A usage group can also define 'free_quantity', the quantity provided for free
with the license. Only usage exceeding this quantity will be charged for.
For counters this means that the free quantity is subtracted from the total quantity.
For gauges this means that the gauge values that equal free_quantity are ignored.

See:
CommerceLicenseBillingUsageInterface
commerce_license_billing_usage_add()
commerce_license_billing_usage_clear()
commerce_license_billing_current_usage()
commerce_license_billing_usage_list()

Recurring order refresh
-----------------------
The recurring order is refreshed each time it is loaded (hook_commerce_order_load()),
updating the line items (quantities, prices) based on the latest plan history and usage.

Billing cycle types
-------------------
Billing cycle types are exportable entities managed on admin/config/licenses/billing-cycle-types.
Each billing cycle type has a bundle, the billing cycle engine, powered by
<a href="a href="https://drupal.org/project/entity_bundle_plugin">entity bundle plugin</a>.
This allows each billing cycle type to have methods for creating and naming new billing cycles.
See CommerceLicenseBillingCycleEngineInterface and CommerceLicenseBillingCycleTypeBase
for more information.

The module provides a "periodic" billing cycle engine that generates periodic
billing cycles (daily/weekly/monthly, synchronous or asynchronous).

Fields and bundles
------------------
The module creates the following fields on any product type that's license enabled:
- cl_billing_cycle_type - Reference to the billing cycle type.
- cl_payment_type - The payment type.

It provides the recurring line item type with the following additional fields:
- cl_billing_start - a datetime field, used for prorating.
- cl_billing_end - a datetime field, used for prorating.
- cl_billing_license - Reference to the license for which the line item was generated.

It provides the recurring order type with the following additional fields:
- cl_billing_cycle - Reference to the billing cycle.

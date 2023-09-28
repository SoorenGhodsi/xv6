static unsigned random_seed = 1;

#define RANDOM_MAX ((1u << 31u) - 1u)
unsigned lcg_parkmiller(unsigned *state)
{
    const unsigned N = 0x7fffffff;
    const unsigned G = 48271u;

    /*  
        Indirectly compute state*G%N.

        Let:
          div = state/(N/G)
          rem = state%(N/G)

        Then:
          rem + div*(N/G) == state
          rem*G + div*(N/G)*G == state*G

        Now:
          div*(N/G)*G == div*(N - N%G) === -div*(N%G)  (mod N)

        Therefore:
          rem*G - div*(N%G) === state*G  (mod N)

        Add N if necessary so that the result is between 1 and N-1.
    */
    unsigned div = *state / (N / G);  /* max : 2,147,483,646 / 44,488 = 48,271 */
    unsigned rem = *state % (N / G);  /* max : 2,147,483,646 % 44,488 = 44,487 */

    unsigned a = rem * G;        /* max : 44,487 * 48,271 = 2,147,431,977 */
    unsigned b = div * (N % G);  /* max : 48,271 * 3,399 = 164,073,129 */

    return *state = (a > b) ? (a - b) : (a + (N - b));
}

unsigned next_random() {
    return lcg_parkmiller(&random_seed);
}

/* Return a random number from 0 to max */
unsigned random_at_most(unsigned max) {
  unsigned num_bins = (max + 1);
  unsigned num_rand = RANDOM_MAX + 1;
  unsigned bin_size = num_rand / num_bins;
  unsigned defect = num_rand % num_bins;
  unsigned retval;
  unsigned x;

  do
    x = next_random();
  while (num_rand - defect <= x);
  
  retval = x/bin_size;
  return retval;
}

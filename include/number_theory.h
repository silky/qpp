/*
 * Quantum++
 *
 * Copyright (c) 2013 - 2014 Vlad Gheorghiu (vgheorgh@gmail.com)
 *
 * This file is part of Quantum++.
 *
 * Quantum++ is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Quantum++ is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Quantum++.  If not, see <http://www.gnu.org/licenses/>.
 */

/**
* \file number_theory.h
* \brief Number theory functions
*/

#ifndef NUMBER_THEORY_H_
#define NUMBER_THEORY_H_

// number theory functions

namespace qpp
{
/**
* \brief Simple continued fraction expansion
* \see qpp::contfrac2x()
*
* \param x Real number
* \param n Number of terms in the expansion
* \param cut Stop the expansion when the next term is greater than \a cut
* \return Integer vector containing the simple continued fraction expansion
* of \a x. If there are \a m less than \a n terms in the expansion,
* a shorter vector with \a m components is returned.
*/
std::vector<long long int> x2contfrac(double x, idx n,
        idx cut = 1e5)
{
    if (n == 0)
        throw Exception("qpp::x2contfrac()", Exception::Type::OUT_OF_RANGE);

    std::vector<long long int> result;

    for (idx i = 0; i < n; ++i)
    {
        result.push_back(std::llround(std::floor(x)));
        x = 1 / (x - std::floor(x));
        if (!std::isfinite(x) || x > cut)
            return result;
    }

    return result;
}

/**
* \brief Real representation of a simple continued fraction
* \see qpp::x2contfrac()
*
* \param cf Integer vector containing the simple continued fraction expansion
* \param n Number of terms considered in the continued fraction expansion.
* If \a n is greater than the size of \a cf,then all terms in \a cf
* are considered.
* \return Real representation of the simple continued fraction
*/
double contfrac2x(const std::vector<int>& cf, idx n)
{
    if (cf.size() == 0)
        throw Exception("qpp::contfrac2x()", Exception::Type::ZERO_SIZE);

    if (n == 0)
        throw Exception("qpp::contfrac2x()", Exception::Type::OUT_OF_RANGE);

    if (n > cf.size())
        n = cf.size();

    if (n == 1) // degenerate case, integer
        return cf[0];

    double tmp = 1. / cf[n - 1];
    for (idx i = n - 2; i != 0; --i)
    {
        tmp = 1. / (tmp + cf[i]);
    }

    return cf[0] + tmp;
}

/**
* \brief Real representation of a simple continued fraction
* \see qpp::x2contfrac()
*
* \param cf Integer vector containing the simple continued fraction expansion
* \return Real representation of the simple continued fraction
*/
double contfrac2x(const std::vector<int>& cf)
{
    if (cf.size() == 0)
        throw Exception("qpp::contfrac2x()", Exception::Type::ZERO_SIZE);

    if (cf.size() == 1) // degenerate case, integer
        return cf[0];

    double tmp = 1. / cf[cf.size() - 1];
    for (idx i = cf.size() - 2; i != 0; --i)
    {
        tmp = 1. / (tmp + cf[i]);
    }

    return cf[0] + tmp;
}

/**
* \brief Greatest common divisor of two non-negative integers
* \see qpp::lcm()
*
* \param m Non-negative integer
* \param n Non-negative integer
* \return Greatest common divisor of \a m and \a n
*/
idx gcd(idx m, idx n)
{
    if (m == 0 || n == 0)
        return (std::max(m, n));

    idx result = 1;
    while (n)
    {
        result = n;
        n = m % result;
        m = result;
    }

    return result;
}

/**
* \brief Greatest common divisor of a list of non-negative integers
* \see qpp::lcm()
*
* \param ns List of non-negative integers
* \return Greatest common divisor of all numbers in \a ns
*/
idx gcd(const std::vector <idx>& ns)
{
    if (ns.size() == 0)
        throw Exception("qpp::gcd()", Exception::Type::ZERO_SIZE);

    idx result = ns[0]; // convention: gcd({n}) = n
    for (idx i = 1; i < ns.size(); ++i)
    {
        result = gcd(result, ns[i]);
    }

    return result;
}

/**
* \brief Least common multiple of two positive integers
* \see qpp::gcd()
*
* \param m Positive integer
* \param n Positive integer
* \return Least common multiple of \a m and \a n
*/
idx lcm(idx m, idx n)
{
    if (m == 0 || n == 0)
        throw Exception("qpp::lcm()", Exception::Type::OUT_OF_RANGE);

    return m * n / gcd(m, n);
}

/**
* \brief Least common multiple of a list of positive integers
* \see qpp::gcd()
*
* \param ns List of positive integers
* \return Least common multiple of all numbers in \a ns
*/
idx lcm(const std::vector <idx>& ns)
{
    if (ns.size() == 0)
        throw Exception("qpp::lcm()", Exception::Type::ZERO_SIZE);

    if (ns.size() == 1) // convention: lcm({n}) = n
        return ns[0];

    if (std::find(std::begin(ns), std::end(ns), 0) != std::end(ns))
        throw Exception("qpp::lcm()", Exception::Type::OUT_OF_RANGE);

    auto multiply = [](idx x, idx y) -> idx
    {
        return x * y;
    };

    idx prod = std::accumulate(std::begin(ns), std::end(ns),
            1u, multiply);

    return prod / gcd(ns);
}

/**
* \brief Inverse permutation
*
* \param perm Permutation
* \return Inverse of the permutation \a perm
*/
std::vector <idx> invperm(const std::vector <idx>& perm)
{
    if (!internal::_check_perm(perm))
        throw Exception("qpp::invperm()", Exception::Type::PERM_INVALID);

    // construct the inverse
    std::vector <idx> result(perm.size());
    for (idx i = 0; i < perm.size(); ++i)
        result[perm[i]] = i;

    return result;
}

/**
* \brief Compose permutations
*
* \param perm Permutation
* \param sigma Permutation
* \return Composition of the permutations \a perm \f$\circ\f$ \a sigma
*  = perm(sigma)
*/
std::vector <idx> compperm(const std::vector <idx>& perm,
        const std::vector <idx>& sigma)
{
    if (!internal::_check_perm(perm))
        throw Exception("qpp::compperm()", Exception::Type::PERM_INVALID);
    if (!internal::_check_perm(sigma))
        throw Exception("qpp::compperm()", Exception::Type::PERM_INVALID);
    if (perm.size() != sigma.size())
        throw Exception("qpp::compperm()", Exception::Type::PERM_INVALID);

    // construct the composition perm(sigma)
    std::vector <idx> result(perm.size());
    for (idx i = 0; i < perm.size(); ++i)
        result[i] = perm[sigma[i]];

    return result;
}

} /* namespace qpp */

#endif /* NUMBER_THEORY_H_ */

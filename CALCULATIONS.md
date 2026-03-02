### Disclaimer
I am not formally trained in GR - this project is based on self-study. The derivations below are my best understanding of the black hole physics, even though they appear to be correct - they should not be treated as a reference.

# Schwarzschild metric
We begin with the Schwarzschild spacetime written in Schwarzschild
coordinates $(t, r, \theta, \phi)$

The line element is
$$
ds^2 = \left(1 - \frac{r_s}{r}\right) c^2 dt^2 - \left(1 - \frac{r_s}{r}\right)^{-1} dr^2 - r^2 d\theta^2 - r^2 \sin^2 \theta \, d\phi^2
$$
where the Schwarzschild radius is $r_s = \frac{2GM}{c^2}$.

### Units
For the sake of simplicity and computational convenience, we adapt geometrized units $G = c = 1$

The Schwarzschild radius becomes $r_s = 2M$

### Symmetry
Schwarzschild black holes are spherically symmetric.  
For motion confined to a plane (equatorial plane), we can set $\theta = \pi/2$, so $d\theta = 0$.



### Simplified metric
$$
ds^2 = \left(1 - \frac{2M}{r}\right)dt^2 - \left(1 - \frac{2M}{r}\right)^{-1} dr^2 - r^2 d\phi^2
$$
(using the $(+,−,−,−)$ signature).

Metric components 
$$
g_{tt} ​= \left(1 - \frac{2M}{r}\right)\\
g_{rr} = -\left(1 - \frac{2M}{r}\right)^{−1}\\
g_{\phi\phi} = -r^2
$$

### Conserved quantities
We obtain two conserved quantities along geodesics.

Energy is $E = g_{tt} \frac{dt}{d\lambda}$, so $\frac{dt}{d\lambda} = E\left(1 - \frac{2M}{r}\right)^{-1}$

Angular momentum is $L = r^2 \frac{d\phi}{d\lambda}$, so $\boxed{\frac{d\phi}{d\lambda} = Lr^{-2}}$

And impact parameter which is $b = \frac{L}{E}$

### Null geodesic
For photons, $ds^2 = 0$. 

Substituting derivatives with respect to the affine parameter $\lambda$
$$
0 = \left(1 − \frac{2M}{r}​\right) \left(\frac{dt}{d\lambda}​\right)^2 - \left(1 - \frac{2M}{r}\right)^{−1} \left(\frac{dr}{d\lambda}​\right)^2 − r^2\left(\frac{d\phi}{d\lambda}\right)^2
$$

Substituting the expressions for $\frac{dt}{d\lambda}$ and $\frac{d\phi}{d\lambda}$

$$
0 = \left(1 − \frac{2M}{r}​\right)^{-1} E^2 - \left(1 - \frac{2M}{r}\right)^{−1} \left(\frac{dr}{d\lambda}​\right)^2 − \frac{L^2}{r^2}
$$

After multiplying by $\left(1 - \frac{2M}{r}\right)$ and rearranging

$$
\left(\frac{dr}{d\lambda}​\right)^2 = E^2 - \left(1 − \frac{2M}{r}​\right)\frac{L^2}{r^2}
$$

### Obtaining the ODE
If we differentiate $\left(\frac{dr}{d\lambda}​\right)^2$ with respect to $\lambda$, using the chain rule

$$
2\frac{dr}{d\lambda}\frac{d^2r}{d\lambda^2} = \frac{dr}{d\lambda} \left(\frac{2L^2}{r^3} - \frac{6ML^2}{r^4}\right)
$$

Simplifying 

$$
\boxed{\frac{d^2r}{d\lambda^2} = \frac{L^2}{r^3}\left(1 - \frac{3M}{r}\right)}
$$

This equation has a zero at $r = 3M$ - the photon sphere, where light can orbit the black hole

### Runge-Kutta 4 system

We normalize the affine parameter such that $E = 1$, and since $E = 1$ we also have $L = b$

We can turn the obtained second-order ODEs into two first-order ones. Remembering $\frac{d\phi}{d\lambda} = \frac{L}{r^2}$, our system is:

$$
\frac{dr}{d\lambda} = v_r\\
\frac{d{v_r}}{d\lambda} = \frac{b^2}{r^3} - \frac{3Mb^2}{r^4}\\
\frac{d\phi}{d\lambda} = \frac{b}{r^2}
$$

So the state vector is $(r, v_r, \phi)$

With initial conditions

$$
r_0 = r_0\\
\phi_0 = 0\\
v_{r0} = \pm\sqrt{1 - \frac{b^2}{r^2_0}\left(1 - \frac{2M}{r_0}\right)}
$$

Where $r0$ is the initial distance from the camera to the origin; In $v_{r0}$ the sign of the root depends on the direction of the ray: $-$ if the ray moves inward and $+$ if outward
==============================================
spice file netlist fixer
==============================================

HOW TO USE
===============

Just run ``make``, and execute ``build/spice_file_netlist_fixer``

.. code-block:: text

    make

Normally could see

.. code-block:: text

    Building  -> pre-build ...
    Building spice_file_netlist_fixer.c -> build/spice_file_netlist_fixer.o ...
    gcc -c -o build/spice_file_netlist_fixer.o spice_file_netlist_fixer.c -I. -lm
    gcc -o build/spice_file_netlist_fixer build/spice_file_netlist_fixer.o -I. -lm
    Building  -> post-build ..

And then, run ``spice_file_netlist_fixer`` in build folder, you could get

.. code-block:: text

    ./build/spice_file_netlist_fixer < orig.spi > new.spi

void rootlogon() {
    // ── Include paths ────────────────────────────────────────────
    gSystem->AddIncludePath("-I/home/ydenizhernandez/ModularEarth/include");
    gSystem->AddIncludePath("-I/home/ydenizhernandez/ModularEarth/src");

    // ── Compiler flags ───────────────────────────────────────────
    gSystem->AddLinkedLibs("-L/home/ydenizhernandez/ModularEarth/src");

    cout << "[rootlogon] ModularEarth paths loaded." << endl;
}

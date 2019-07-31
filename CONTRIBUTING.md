# Contributing

We are happy to accept any contributions that make sense and respect the rules listed below.

## How to contribute

1. Fork the repo.
2. Create a feature branch for your contribution out of the `master` branch. Only one contribution per branch is accepted.
3. Implement your contribution while respecting our rules (see below).
4. If possible, add tests for your contribution to make sure it actually works.
5. Submit a pull request against our `master` branch!

## Rules

* **Do** use feature branches.
* **Do** conform to existing coding style so that your contribution fits in.
* **Do not** commit any generated files, unless already in the repo. If absolutely necessary, explain why.
* **Do not** create any top level files or directories. If absolutely necessary, explain why.

## License

By contributing your code, you agree to license your contribution under our [LICENSE](License.md).

## Caveats

Some parts of VTS require that native executables be used. Currently the build system does not support this. If your contribution requires rebuilding of these assets, please denote this in the pull req and I will build the asset after merging the PR.

If changing an existing test, you must set mTestWasChanged to true in ApplicationUpdateBroadcastReceiver. This notifies the user that an existing test has changed.

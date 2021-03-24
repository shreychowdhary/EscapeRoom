set -euxo pipefail

(cd client && yarn && yarn eslint)
(cd server && yarn && yarn eslint)